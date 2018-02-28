#!/usr/local/bin/perl -w
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
#
# NAME
#   export_doc.pm  -  Creates docbook XML Documentation
#

package export_doc;
require Exporter;
@ISA = qw(Exporter);

# Trap errors as soon as possible
use strict;

# Useful packages
use Getopt::Std;
use FileHandle;
use Data::Dumper;

# Command Line Arguments
my ($opt_out, $opt_build);

# Structure for storing PSKey data
my (@data);

my $ofh;

my $chapter_number = 1;

my $xml_header = <<EOT;
<?xml version="1.0"?>
<!DOCTYPE Manual PUBLIC "-//OVIDIUS//DTD MANUAL V1.0 CSR//EN" "Manual.dtd">
<Manual DOCUMENT.NUMBER="202076" DOCUMENT.TYPE="Procedure" ISSUE.NUMBER="3"
 REVISION.NUMBER="3" TableOfContents="yes" TableOfFigures="no" TableOfTables="no"><Manual-Head>
<PubInfo><ProductName>ProductName</ProductName>
<DocTitle>Document Title</DocTitle><DocStatus/><ProductCode/>
<Sidebar>ProductName Document Title</Sidebar><Watermark/>
<Footer><FooterPara>CSR Internal</FooterPara><FooterPara>(c) Cambridge Silicon
Radio Limited 2012</FooterPara></Footer></PubInfo>
<DocHistory anchor="d1">
<History><Revision>1</Revision><Date>DD MMM YY</Date><ChangeReason>
<Paragraph>Change reason
paragraph</Paragraph></ChangeReason></History></DocHistory></Manual-Head>
<Manual-Body>


EOT

my $xml_footer = <<EOT;
</Manual-Body>
</Manual>
EOT



# ******** Start of Functions ************

sub open_doc {
    my ($output_dir, $output_file, $template_file) = @_;
    
    # Open output file
    open($ofh, ">$output_dir$output_file") or die "Can't open $output_dir$output_file: $!";

    # Try to open template file
    if (open FILE, "<", $output_dir . $template_file) {
        ($xml_header, $xml_footer) = split (/<Chapter anchor="auto-generated"><Title>Auto-generated<\/Title><\/Chapter>/, join("", <FILE>));  
    }

    # Add the XML header
    print $ofh $xml_header;

    $opt_out = $output_dir . $output_file;

}

sub start_chapter {
    my ($build_info) = @_;
    # Add title
    $build_info =~ s/wlan/WLAN/;
    $build_info =~ s/fw/firmware/;
    create_title($ofh, "Configuration List for $build_info");
}

sub add_config_element {
   my ($elem, $unfiltered) = @_;
   
   # check for "CSR only" type comments
   my $internal = 0;
   if (($elem->{description_user} =~ m/CSR\s(use|internal\suse)\sonly/i) ||
       ($elem->{description_user} =~ m/(advice|guidance)\s(of|from)\sCSR/i) ||
       ($elem->{description_user} =~ m/(instruction\sby|consulting)\sCSR/i) ||
       ($elem->{description_user} =~ m/instructed\sto\sdo\sso\sby\sCSR/i) ) {
      # $internal = 1; # Disable this for now since customers usually get all the keys
   }
   
   if (defined($unfiltered) || (($elem->{is_internal} !~ /true/i) && (!$internal)) ) {
      $elem->{collatekey} = collation_key($elem->{name});
      $elem->{hexnum} = sprintf("0x%04x", $elem->{psid});
      push(@data, $elem);
   }
}

sub add_chapter {
    my ($unfiltered) = @_;

    # Sort data numerically
    my (@numkeys) = sort {$a->{psid} <=> $b->{psid}} @data;

    # Sort Data alphabetically
    my (@alphakeys) = sort {$a->{collatekey} cmp $b->{collatekey}} @data;

    compile_cross_references();

    # Add the index links
    # create_index($ofh, "Configuration Elements Listed Alphabetically", "alphabetic$chapter_number",  @alphakeys); 
    create_index($ofh, "Configuration Elements Listed Numerically", "numeric$chapter_number", @numkeys);
    
    print $ofh "<Chapter anchor=\"Config_elements$chapter_number\"><Title>Configuration Elements</Title>\n";

    # Add a table for each key
    foreach my $key (@alphakeys) {
      insert_table($ofh, $key, $unfiltered);
    }
    print $ofh "</Chapter>";
if (0) {
    # not easy to generate permuted index without text links
    create_permuted_index($ofh);
}
    @data=();
    print $ofh "</Chapter>";
    $chapter_number++;
}

sub close_doc {

   # Finish off the XML file
   
   print $ofh $xml_footer;

   # Close the output file
   close($ofh);
   print "Created $opt_out\n";
}

# Generate a string suitable for collation.
#
# We'd like collation sequences like
#
#     bash
#     bat_wingspan
#     bath
#     bauble_1
#     bauble_5
#     bauble_23
#
# Also, we ignore case and sort all strings starting with punctuation
# together at the end.

sub collation_key {
  my($n) = @_;

  $n = uc($n);                                 # Ignore case
  $n = chr(127) . "$n" if ($n !~ /^[0-9A-Z]/); # Sort all puntuation together
  $n =~ tr/_/ /;                               # ' ' < '[0-9A-Z]'
  $n =~ s/(\d+)/sprintf("%09d", $1)/eg;        # Expand digit sequences

  return $n;
}

# Make compiled substitution expression for key names.
my ($cross_reference);
sub compile_cross_references {
  my($code) = "";

  foreach my $k (@data) {
    $code .=
      "\$str =~ s/\\b$k->{name}\\b/" .
        "<a href=\"\#$k->{name}\">$k->{name}<\\/a>/g;"
  }

  $cross_reference = eval "sub { my(\$str) = \@_; $code; \$str;}";
}


#************************************************************************
# HTML structures


# Function to quote a text string so it displays in HTML.

sub html_quote
{
    my ($str) = @_;
   
    # First unescape any HTML escape sequences
    $str =~ s/&amp;/&/gi;
    $str =~ s/&quot;/\"/gi;
    $str =~ s/&lt;/</gi;
    $str =~ s/&gt;/>/gi;
    
    # Now escape any special chars
    $str =~ s/&/&amp;/g;
    $str =~ s/\"/&quot;/g;
    $str =~ s/</&lt;/g;
    $str =~ s/>/&gt;/g;

    # remove spaces from lines only containing spaces
    $str =~ s/\n\s+\n/\n\n/g;

    # replace leading spaces with non-breakable space chars to preserve layout
    # $str =~ s/^( +)/" " x length($1)/gme;

    # replace leading tabs with non-breakable space chars to preserve layout
    $str =~ s/^(\t+)/" " x 8 x length($1)/gme;

    # convert control chars other than tab and new line to their HTML escaped version
    $str =~ s/([^\t\n\x20-\x7e])/sprintf "&#%03d;", (ord $1)/eg;
    
    # Now revert HTML tags
    $str =~ s!&lt;b&gt;!<Emphasis type="bold">!gi;
    $str =~ s!&lt;/b&gt;!</Emphasis>!gi;
    $str =~ s!&lt;i&gt;!<Emphasis type="italic">!gi;
    $str =~ s!&lt;/i&gt;!</Emphasis>!gi;
    $str =~ s!&lt;u&gt;!<Emphasis>!gi;  # Underline?
    $str =~ s!&lt;/u&gt;!</Emphasis>!gi;
    $str =~ s!&lt;strike&gt;!<strike>!gi;
    $str =~ s!&lt;/strike&gt;!</strike>!gi;
    $str =~ s!&lt;sub&gt;!<sub>!gi;
    $str =~ s!&lt;/sub&gt;!</sub>!gi;
    $str =~ s!&lt;sup&gt;!<sup>!gi;
    $str =~ s!&lt;/sup&gt;!</sup>!gi;
    $str =~ s!&lt;tt&gt;!<tt>!gi;
    $str =~ s!&lt;/tt&gt;!</tt>!gi;
    $str =~ s!&lt;em&gt;!<em>!gi;
    $str =~ s!&lt;/em&gt;!</em>!gi;
    $str =~ s!&lt;strong&gt;!<strong>!gi;
    $str =~ s!&lt;/strong&gt;!</strong>!gi;
    $str =~ s!&lt;p&gt;!<p>!gi;
    $str =~ s!&lt;/p&gt;!</p>!gi;
    $str =~ s!&lt;br&gt;!<br>!gi;
    $str =~ s!&lt;ul&gt;!<ul>!gi;
    $str =~ s!&lt;/ul&gt;!</ul>!gi;
    $str =~ s!&lt;ol&gt;!<ol>!gi;
    $str =~ s!&lt;/ol&gt;!</ol>!gi;
    $str =~ s!&lt;dl&gt;!<dl>!gi;
    $str =~ s!&lt;/dl&gt;!</dl>!gi;
    $str =~ s!&lt;li&gt;!<li>!gi;
    $str =~ s!&lt;/li&gt;!</li>!gi;

    $str;
}

# Function to quote a text block so it displays reasonably in HTML.

sub doc_quote_block
{
    my ($str) = @_;

    $str = html_quote($str);
    if (0) { # OK for WLAN
        $str =~ s,\n*$,,;
        $str =~ s,\n ,\n</Paragraph><Paragraph>\n ,g;
        $str =~ s,\n\n,\n</Paragraph>\n<Paragraph>\n,g;
        $str =~ s,<p>\n<br>,<Paragraph>,g;
        $str =~ s,<br>(\n|<br>)*<br>,</Paragraph>\n<Paragraph>,g;
        $str =~ s,(</Paragraph>\n<Paragraph>)*$,,;
        $str =~ s,\n([^<]),\n    $1,g;
        $str =~ s,(</*(Paragraph)>),   $1,g;
    }
    else {
        my $outstr = "";
        my $in_code = 0;
        my $in_pre = 0;
        foreach my $line (split(/\n/,$str)) {
            if ($line =~ s/^&lt;pre&gt;//) {
                $outstr .= "<Code>\n";
                $in_pre = 1;
            }
            if (!$in_pre) {
                if ($line =~ /^ /) {
                    if (!$in_code) {
                         $outstr .= "<Code>\n";
                         $in_code = 1;
                    }
                }
                else {
                    if ($in_code) {
                         $outstr .= "</Code>";
                         $in_code = 0;
                    }
                }
            }
            if ($line =~ s/^&lt;\/pre&gt;//) {
                $outstr .= "</Code>\n";
                $in_pre = 0;
            }

            if ($line eq "") {
                $outstr .= "</Paragraph><Paragraph>";
            }
            $outstr .= $line . "\n";
        }
        if ($in_code) {
            $outstr .= "</Code>";
        }
        $str = $outstr;
    }
    $str =~ s,&lt;br[ \/]*&gt;,</Paragraph><Paragraph>,g;
    # Replace any references to other PSKeys with hotlinks
    &$cross_reference($str);
}

sub html_valueentry
{
   my $str="";
#print "DBG 0 $_[0] 1 $_[1] 2 $_[2]\n";

   # If no value for MINIMUM or MAXIMUM was found, $_[2] won't be valid
   # in calling function, but as it has been passed down will be partly
   # valid here.  Trapping the warning ref uninitialised avoids the need
   # to check above.
   local $SIG{__WARN__} = sub {
                                if ($_[0] =~ 'uninitialized')
                                {
                                   $str = "";
                                }
                                else
                                {
                                   warn $_[0];
                                }
                              };
   if ($_[2] ne "")
   {
   $str = $_[0].
      "  <td width=\"15%\" style='width:15%' align=right><b>$_[1]</b></td>\n".
      "  <td width=\"15%\" style='width:15%' align=left>$_[2]</td>\n".
      " </tr>\n"
   }
   $str;
}

# This function inserts a config element table.
#
sub insert_table
{
    my($fh, $k, $unfiltered) = @_;

    my $total_columnspan = 3;
    my $compound_type = $k->{"type"};
    if (defined($k->{is_array})) {
       if ($k->{is_array} == 1) {
           if (defined($k->{array_length_min}) && defined($k->{array_length_max}) && ($k->{array_length_min} == $k->{array_length_max})) {
               $compound_type .= "[$k->{array_length_min}]";
           } elsif (defined($k->{array_length_min}) && defined($k->{array_length_max})) {
               $compound_type .= "[$k->{array_length_min}..$k->{array_length_max}]";
           } elsif (defined($k->{array_length_max})) {
               $compound_type .= "[0..$k->{array_length_max}]";
           } else {
               $compound_type .= "[ ]";
           }
       }
    }
    print $fh <<EOT;
<Chapter anchor="$k->{name}"><Title>... $k->{name}</Title>
<Description>
<Table anchor="Table-$k->{name}"><Tgroup cols="3"><Colspec colnum="1" colname="col1"
colwidth="*"/><Colspec colnum="2" colname="col2" colwidth="*"/><Colspec
colnum="3" colname="col3" colwidth="*"/><Tbody>
<Row>
<Entry colname="col1"><Paragraph>PSID</Paragraph></Entry>
<Entry colname="col2"><Paragraph>$k->{hexnum}</Paragraph></Entry>
<Entry colname="col3"><Paragraph>$k->{psid}</Paragraph></Entry>
</Row>
EOT

    if (defined ($k->{category})) {
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Category</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{category}</Paragraph></Entry></Row>\n";
    }

    print $fh "<Row><Entry colname=\"col1\"><Paragraph>Type</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$compound_type</Paragraph></Entry></Row>\n";


    if (defined ($k->{table_name})) {
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Table / Indices</Paragraph></Entry>";
       print $fh "<Entry colname=\"col2\"><Paragraph>$k->{table_name}</Paragraph></Entry>";
       print $fh "<Entry colname=\"col3\"><Paragraph>";
       print $fh "Index 1 = $k->{index1}" if defined $k->{index1};
       print $fh "</Paragraph><Paragraph>Index 2 = $k->{index2}" if defined $k->{index2};
       print $fh "</Paragraph><Paragraph>Index 3 = $k->{index3}" if defined $k->{index3};
       print $fh "</Paragraph></Entry></Row>\n";
    }

    print $fh "<Row><Entry colname=\"col1\"><Paragraph>Default</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>";

    my $num_indexed_defaults = @{$k->{default}};
    

    my $index = 1;
    foreach my $default ( sort {$a->{index1_value} <=> $b->{index1_value} ||
                                $a->{index2_value} <=> $b->{index2_value} ||
                                $a->{index3_value} <=> $b->{index3_value}   } @{$k->{default} }) {
        if ((defined $default->{index1_value}) || (defined $default->{index2_value}) || (defined $default->{index3_value})) {
            print $fh ".$default->{index1_value}" if defined $default->{index1_value};
            print $fh ".$default->{index2_value}" if defined $default->{index2_value};
            print $fh ".$default->{index3_value}" if defined $default->{index3_value};
            print $fh " = ";
        }
        else
        {
            my $temp = $default->{string};
            my $count = ($temp =~ tr/{//);
            if ( $count> 1) {
               print $fh "</Paragraph><Paragraph>";
           }
        }
        
        if ($k->{type} eq "unicode_string") {
           print $fh doc_quote_block('"' . $default->{string} . '"');
        }
        else {
           print $fh doc_quote_block($default->{string});
        }
        
        if (defined ($k->{units})) {
           if ($k->{units} =~ /^[0-9]/) {
               print $fh " x";
           }
           print $fh " " . $k->{units};
        }
        print $fh "</Paragraph><Paragraph>" if $index++ < $num_indexed_defaults;
    }
    if ($index == 1) {
        print $fh "None";
    }    
    print $fh "</Paragraph></Entry></Row>\n";
    if (defined $k->{units}) {
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Units</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{units}</Paragraph></Entry></Row>\n";
    }


##### Allowed values block


    my $columnsForValues = 4;
    my $haveIncludeFile = 0;
    my @enums = sort {$a->{enum_value} <=> $b->{enum_value}} @{$k->{enums}};
    my $numEnums = @enums;

    if ((($numEnums > 0) || defined($k->{range_min}) || defined($k->{range_max})) && ($compound_type ne "bool") && ($compound_type ne "boolean"))
    {
       print $fh "<Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph>Allowed Values</Paragraph></Entry></Row>\n";
    }

    if (($k->{type} eq "octet_string") || ($k->{type} eq "unicode_string")) {
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Minimum string length</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{range_min} octets</Paragraph></Entry></Row>\n";
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Maximum string length</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{range_max} octets</Paragraph></Entry></Row>\n";
    }
    else {
       if (defined $k->{range_min}) {
          if ($k->{'format'} eq "hexadecimal") {
             $k->{'type'} =~ m/uint(\d+)/;
             my $hex_chars = $1/4 . "x";
             $k->{range_min} = sprintf("0x%0$hex_chars", $k->{range_min});
          }
          print $fh "<Row><Entry colname=\"col1\"><Paragraph>Minimum</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{range_min}";
          if ($k->{units} =~ /^[0-9]/) {
            print $fh " x";
          }
          print $fh " $k->{units}</Paragraph></Entry></Row>\n";

       }
       if (defined $k->{range_max}) {
          if ($k->{'format'} eq "hexadecimal") {
             $k->{'type'} =~ m/uint(\d+)/;
             my $hex_chars = $1/4 . "x";
             $k->{range_max} = sprintf("0x%0$hex_chars", $k->{range_max});
          }
          print $fh "<Row><Entry colname=\"col1\"><Paragraph>Maximum</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{range_max}";
          if ($k->{units} =~ /^[0-9]/) {
            print $fh " x";
          }
          print $fh " $k->{units}</Paragraph></Entry></Row>\n";
       }
    }


    if (($numEnums > 0) && ($compound_type ne "bool") && ($compound_type ne "boolean")) {
        my $columnsForEnumDesc = $columnsForValues - 1;
        print $fh <<EOT;
<Row>
<Entry colname="col1"><Paragraph><Emphasis type="bold">Defined name</Emphasis></Paragraph></Entry>
<Entry colname="col2"><Paragraph><Emphasis type="bold">Value</Emphasis></Paragraph></Entry>
<Entry colname="col3"><Paragraph><Emphasis type="bold">Description</Emphasis></Paragraph></Entry>
</Row>
EOT

        for (my $i=0; $i < $numEnums; $i++)
        {
            print $fh "<Row><Entry colname=\"col1\"><Paragraph>" . doc_quote_block(@enums[$i]->{enum_label}) . "</Paragraph></Entry>";
            print $fh "<Entry colname=\"col2\"><Paragraph>" . doc_quote_block(@enums[$i]->{enum_value}) . "</Paragraph></Entry>";
            print $fh "<Entry colname=\"col3\"><Paragraph>" . doc_quote_block(@enums[$i]->{enum_description}) . "</Paragraph></Entry></Row>\n";
        }
    }

    if (defined $k->{access_rights}) {
       print $fh "<Row><Entry colname=\"col1\"><Paragraph>Access Rights</Paragraph></Entry><Entry namest=\"col2\" nameend=\"col3\"><Paragraph>$k->{access_rights}</Paragraph></Entry></Row>\n";
    }


##### Main description block

    print $fh "<Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph>";
    if (defined($k->{label})) {
        print $fh "<Emphasis type=\"bold\">";
        print $fh doc_quote_block($k->{label});
        print $fh "</Emphasis></Paragraph><Paragraph>";
    }
    
    if ($k->{description_user} =~ m/^HASH/) {
        warn "HASH in user description field for $k->{name} (caused by no text for description_user field in XML)";
        $k->{description_user} = "NO DESCRIPTION";
    }

    if ($k->{name} eq "unifiFrontEndControlConfigString") {
        my $part1;
        my $part2;
        my $part3;
        my $part4;
        ($part1,$part2) = split( /General signals/, $k->{description_user} );
        ($part2,$part3) = split( /WLAN 2.4GHz \/ Bluetooth signals/, $part2 );
        ($part3,$part4) = split( /WLAN 5GHz signals/, $part3 );
        
        print $fh doc_quote_block($part1);
        print $fh "</Paragraph></Entry></Row><Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph>General signals";
        print $fh doc_quote_block($part2);
        print $fh "</Paragraph></Entry></Row><Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph>WLAN 2.4GHz / Bluetooth signals";
        print $fh doc_quote_block($part3);
        print $fh "</Paragraph></Entry></Row><Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph>WLAN 5GHz signals";
        print $fh doc_quote_block($part4);
    }
    else {
        print $fh doc_quote_block($k->{description_user});
    }

    print $fh "</Paragraph>";

    if (defined($k->{description_internal}) && (defined ($unfiltered))) {
       print $fh "<Paragraph><Emphasis type=\"italic\">CSR internal description</Emphasis></Paragraph><Paragraph>";
       print $fh doc_quote_block($k->{description_internal});
       print $fh "</Paragraph>";
    }

    if (defined($k->{source_reference})||defined($k->{source_default})||defined($k->{additional_information})||($k->{is_internal} =~ /true/i)) {
      print $fh "<Paragraph><Emphasis type=\"italic\">Additional Information</Emphasis></Paragraph>";
      
      if (defined($k->{source_reference})) {
         print $fh "<Paragraph><Emphasis type=\"italic\">Source Reference:</Emphasis> $k->{source_reference}</Paragraph>";
      }
if (0) { # disable oid for customer documentation
      if (defined($k->{oid})) {
         print $fh "<Paragraph><Emphasis type=\"italic\">OID:</Emphasis> $k->{oid}</Paragraph>";
      }
}
      if (defined($k->{source_default})) {
         print $fh "<Paragraph><Emphasis type=\"italic\">Source Default:</Emphasis> $k->{source_default}</Paragraph>";
      }
      if (defined($k->{additional_information})) {
         print $fh "<Paragraph>$k->{additional_information}</Paragraph>";
      }
      if ($k->{is_internal} =~ /true/i) {
         print $fh "<Paragraph><Emphasis type=\"italic\">FOR CSR USE ONLY</Emphasis></Paragraph>";
      }
    }
    print $fh "</Entry></Row>\n";

##### Functions
if (0) { # disable function listing for customer documentation
    if (exists($k->{functions}[0])) {
       print $fh "<Row><Entry namest=\"col1\" nameend=\"col3\"><Paragraph><Emphasis type=\"bold\">Access Functions</Emphasis></Paragraph></Entry></Row>\n";

       foreach my $function (@{$k->{functions}}) {
       
          print $fh "<Row><Entry colname=\"col1\"><Paragraph>$function->{type} function</Paragraph></Entry>";
          print $fh "<Entry colname=\"col2\"><Paragraph>$function->{function_name}</Paragraph></Entry>";
          if ($function->{is_for_vif} =~ m/true/i) {
             print $fh "<Entry colname=\"col3\"><Paragraph>Per-VIF function</Paragraph></Entry></Row>";
          }
          else {
             print $fh "<Entry colname=\"col3\"><Paragraph>Global function</Paragraph></Entry></Row>";
          }
       }

    }
}

    print $fh "</Tbody></Tgroup></Table></Description></Chapter>\n\n\n";
}

sub create_index
{
    my($fh, $title, $anchor, @ordered_keys) = @_;

    # This function creates an index of HTML hotlinks.
    # Link names have " " replaced with "_" for compatibility

    if ($anchor =~ /alphabetic/) {
        print $fh <<EOT;
<Chapter anchor="$anchor"><Title>$title</Title>
<Description>
<Table><Tgroup cols="3"><Colspec colnum="1" colname="col1"
colwidth="4*"/><Colspec colnum="2" colname="col2" colwidth="1*"/><Colspec colnum="3" colname="col3" colwidth="1*"/><Tbody>
EOT
    }
    else {
        print $fh <<EOT;
<Chapter anchor="$anchor"><Title>$title</Title>
<Description>
<Table><Tgroup cols="3"><Colspec colnum="1" colname="col1"
colwidth="1*"/><Colspec colnum="2" colname="col2" colwidth="4*"/><Colspec colnum="3" colname="col3" colwidth="1*"/><Tbody>
EOT
    }


    foreach my $k (@ordered_keys) {
        if ($anchor =~ /alphabetic/) {
            print $fh "<Row><Entry colname=\"col1\"><Paragraph>$k->{name}</Paragraph></Entry>";
            print $fh "<Entry colname=\"col2\"><Paragraph>$k->{hexnum}</Paragraph></Entry>";
        }
        else {
            print $fh "<Row><Entry colname=\"col1\"><Paragraph>$k->{hexnum}</Paragraph></Entry>";
            print $fh "<Entry colname=\"col2\"><Paragraph>$k->{name}</Paragraph></Entry>";
        }

        print $fh "<Entry colname=\"col3\"><Paragraph><RefShort target=\"$k->{name}\"/></Paragraph></Entry></Row>\n";
    }
    print $fh "</Tbody></Tgroup></Table></Description></Chapter>\n\n\n";
}

sub create_title
{
    my($fh, $title) = @_;

    # This function creates the title of the section.

    print $fh <<EOT;
<Chapter anchor="section$chapter_number"><Title>$title</Title>
EOT

}

sub create_permuted_index {
  my($fh) = @_;

  my @index;

  foreach my $k (@data) {
    my $keyname = $k->{name};
    my $contains_PSKEY = ($keyname =~ m/^PSKEY_/);
    my $contains_underscore = ($keyname =~ m/_/);

    my $prefix = "";

    if ($keyname =~ m/^(dot11|unifi|CsrWifiSmePsid|CsrWifiNmePsid)/) {
       $prefix = $1;
    }
    
    $keyname =~ s/^PSKEY_//;
    $keyname =~ s/^dot11//;
    $keyname =~ s/^unifi//;
    $keyname =~ s/^CsrWifiSmePsid//;
    $keyname =~ s/^CsrWifiNmePsid//;

    if (!$contains_underscore) {
       # Add underscores between lower and Uppercase to aid splitting
       $keyname =~ s/([a-z0-9])([A-Z])/$1_$2/g;
       $keyname =~ s/([a-z])([0-9])/$1_$2/g;
       $keyname =~ s/([A-Z0-9])([A-Z])([a-z])/$1_$2$3/g;
    }

    my @post = split /_/, $keyname;
    my @pre = ();
    while(@post) {
      my $word = shift @post;
      
      if ($contains_underscore) {
         if ($contains_PSKEY) {
            push @index, [ collation_key(join(" ", ($word, @post, @pre))),
                           join("_", ("PSKEY", @pre)), join("_", ($word, @post)),
                           $k->{name}, undef, $k->{hexnum}];
         }
         else
         {
            push @index, [ collation_key(join(" ", ($word, @post, @pre))),
                           join("_", (@pre)), join("_", ($word, @post)),
                           $k->{name}, undef, $k->{hexnum}];

         }
      }
      else {
            push @index, [ collation_key(join(" ", ($word, @post, @pre))),
                           join("", ("$prefix", @pre)), join("", ($word, @post)),
                           $k->{name}, undef, $k->{hexnum}];
      }
      push @pre, $word;
    }
  }

  @index = sort {$a->[0] cmp $b->[0]} @index;
  my %letter_starts;

  foreach my $i (@index) {
    my $first = uc(substr($i->[2], 0, 1));
    next if ($first !~ /[0-9A-Z]/);
    $first =~ s/\d/digits/;
    if (!$letter_starts{$first}) {
      $i->[4] = $first;
      $letter_starts{$first} = 1;
    }
  }

  print $fh "<h2><a name=\"permuted_index\">Permuted Index</a></h2>\n";

  print $fh "<p>";
  foreach my $l ("digits", "A".."Z") {
    if ($letter_starts{$l}) {
       if ($l ne "digits") {
          print $fh " <a href=\"#permuted_index_$l\">$l</a>\n";
       }
       else {
          print $fh " <a href=\"#permuted_index_$l\">0-9</a>\n";
       }
    }
    else {
       if ($l ne "digits") {
          print $fh " $l\n";
       }
    }
  }
  print $fh "</p>\n";

  print $fh "<table class=\"unboxed\" border=\"0\">\n";

  foreach my $i (@index) {
    if (defined($i->[4])) {

       if ($i->[4] eq "digits") {
          print $fh <<EOT;
 <tr>
  <th colspan=3>
   <a name=\"permuted_index_$i->[4]\">0-9</a>
  </th>
 </tr>
EOT
      }
      else {
         print $fh <<EOT;
<tr>
 <th colspan=3>
  <a name=\"permuted_index_$i->[4]\">$i->[4]</a>
 </th>
</tr>
EOT
      }
    }
    if ($i->[1] ne "") {
       if ($i->[3] =~ m/_/) {
          print $fh <<EOT;
<tr>
 <td align=right>
  <a href="\#$i->[3]">$i->[1]_</a>
 </td>
 <td>
  <a href="\#$i->[3]">$i->[2]</a>
 </td>
 <td>$i->[5]</td>
EOT
       }
       else {
          print $fh <<EOT;
<tr>
 <td align=right>
  <a href="\#$i->[3]">$i->[1]</a>
 </td>
 <td>
  <a href="\#$i->[3]">$i->[2]</a>
 </td>
<td>$i->[5]</td>
EOT
       }
    }
    else {
       print $fh <<EOT;
 <tr>
  <td></td>
  <td>
   <a href="\#$i->[3]">$i->[2]</a>
  </td>
  <td>$i->[5]</td>
EOT
    }
  }

  print $fh "</table>\n"
}

# return true to use statement that includes this module
1;

