#!/usr/local/bin/perl -w
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
#
# NAME
#   export_html.pm  -  Creates HTML Documentation
#

package export_html;
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

# ******** Start of Functions ************

sub open_doc {
    my ($output_file) = @_;
    # Open output file
    open($ofh, ">$output_file") or die "Can't open $output_file: $!";

    # Add the HTML header
    html_prologue($ofh,  "Config Key List");

    $opt_out = $output_file;

}

sub start_chapter {
    my ($build_info) = @_;
    # Add title
    create_title($ofh, "Configuration List for $build_info",
                       "Configuration elements Listed Alphabetically",
                       "Configuration elements Listed Numerically");
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
    create_index($ofh, "Configuration elements Listed Alphabetically", @alphakeys);
    create_index($ofh, "Configuration elements Listed Numerically", @numkeys);

    # Add a table for each key
    foreach my $key (@numkeys) {
      insert_table($ofh, $key, $unfiltered);
    }
    
    create_permuted_index($ofh);
    @data=();
}

sub close_doc {

   # Finish off the HTML file
   html_epilogue($ofh);

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

# This function adds the HTML header to the page.

sub html_prologue
{
    my ($fh, $title) = @_;

    print $fh <<EOT;
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
       "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>$title</title>
<style type="text/css">
<!--
body, h1, h2, th, td, p, li {font-family:Arial, helvetica, sans-serif;}

h1                  {font-size:12pt;}
h2                  {font-size:10pt;}
h3                  {font-size:10pt;}
body, th, td, p, li {font-size: 9pt;}

table  {border-collapse: collapse; }
th, td {border: solid; border: 1px solid black;}
table.unboxed td, table.unboxed th {border: none; padding-right: 2em;}
-->
</style>
</head>
<body>
EOT
}

# This function adds the HTML footer to the page.

sub html_epilogue
{
    my ($fh) = @_;

    print $fh <<EOT;
</body>
</html>
EOT
}


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
    $str =~ s/^( +)/"&nbsp;" x length($1)/gme;

    # replace leading tabs with non-breakable space chars to preserve layout
    $str =~ s/^(\t+)/"&nbsp;" x 8 x length($1)/gme;

    # convert control chars other than tab and new line to their HTML escaped version
    $str =~ s/([^\t\n\x20-\x7e])/sprintf "&#%03d;", (ord $1)/eg;
    
    # Now revert HTML tags
    $str =~ s!&lt;b&gt;!<b>!gi;
    $str =~ s!&lt;/b&gt;!</b>!gi;
    $str =~ s!&lt;i&gt;!<i>!gi;
    $str =~ s!&lt;/i&gt;!</i>!gi;
    $str =~ s!&lt;u&gt;!<u>!gi;
    $str =~ s!&lt;/u&gt;!</u>!gi;
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
    $str =~ s!&lt;pre&gt;!<pre>!gi;
    $str =~ s!&lt;/pre&gt;!</pre>!gi;

    $str;
}

# Function to quote a text block so it displays reasonably in HTML.

sub html_quote_block
{
    my ($str) = @_;

    $str = html_quote($str);
    $str =~ s,\n*$,,;
    $str =~ s,\n&nbsp;,\n<br>\n&nbsp;,g;
    $str =~ s,\n\n,\n</p>\n<p>\n,g;
    $str =~ s,<p>\n<br>,<p>,g;
    $str =~ s,<br>(\n|<br>)*<br>,</p>\n<p>,g;
    $str =~ s,(</p>\n<p>)*$,,;
    #$str =~ s,\n([^<]),\n    $1,g;   # This screwed up pre-formatted text
    #$str =~ s,(</*(p|br)>),   $1,g;

    # The following block of code removes <br> lines that have been added to preformatted text
        my $outstr = "";
        my $in_pre = 0;
        foreach my $line (split(/\n/,$str)) {
            if ($line =~ s/^&lt;pre&gt;//) {
                $in_pre = 1;
            }

            if ($line =~ s/^&lt;\/pre&gt;//) {
                $in_pre = 0;
            }

            if ($line ne "<br>") {
                $outstr .= $line . "\n";
            }
        }
        $str = $outstr;
    
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
    # Document can be converted to HTML 4.01 strict by removing the
    # width attribute.  The style attribute performs the same function
    # but the older width= is provided for compatibility
    print $fh <<EOT;
<h1><a name="$k->{name}"></a>$k->{name}</h1>
<table width="100%" border="1" bordercolor="black" cellspacing="0" cellpadding="5">
 <tr align=center valign=top>
  <td width="20%" style='width:20%' align=left>Number (psid)</th>
  <td width="20%" style='width:20%' align=left>$k->{hexnum}</th>
  <td width="60%" style='width:60%' align=left>$k->{psid}</th>
 </tr>
EOT

    if (defined ($k->{category})) {
       print $fh "<tr><td>Category</td><td colspan=2>$k->{category}</td></tr>";
    }

    print $fh "<tr><td>Type</td><td colspan=2>$compound_type</td></tr>";


    if (defined ($k->{table_name})) {
       print $fh "<tr><td>Table / Indices</td><td>$k->{table_name}</td><td>";
       print $fh "Index 1 = $k->{index1}" if defined $k->{index1};
       print $fh "<br>Index 2 = $k->{index2}" if defined $k->{index2};
       print $fh "<br>Index 3 = $k->{index3}" if defined $k->{index3};
       print $fh "</td></tr>";
    }

    print $fh "<tr><td>Default</td><td colspan=2>";

    my $num_indexed_defaults = @{$k->{default}};
    

    my $index = 1;
    foreach my $default ( sort {$a->{index1_value} <=> $b->{index1_value} ||
                                $a->{index2_value} <=> $b->{index2_value} ||
                                $a->{index3_value} <=> $b->{index3_value}   } @{$k->{default} }) {
        if ($num_indexed_defaults > 1) {
            print $fh "$k->{name}";
            print $fh ".$default->{index1_value}" if defined $default->{index1_value};
            print $fh ".$default->{index2_value}" if defined $default->{index2_value};
            print $fh ".$default->{index3_value}" if defined $default->{index3_value};
            print $fh " = ";
        }
        else
        {
            if ($k->{is_array} == 1) {
               print $fh "$k->{name}\[ \] = ";
            }
            else {
               print $fh "$k->{name} = ";
               
               my $temp = $default->{string};
               my $count = ($temp =~ tr/{//);
               if ( $count> 1) {
                  print $fh "<br>";
               }
            }
        }
        
        if ($k->{type} eq "unicode_string") {
           print $fh html_quote_block('"' . $default->{string} . '"');
        }
        else {
           print $fh html_quote_block($default->{string});
        }
        
        if (defined ($k->{units})) {
           if ($k->{units} =~ /^[0-9]/) {
               print $fh " x";
           }
           print $fh " " . $k->{units};
        }
        print $fh "<br>" if $index++ < $num_indexed_defaults;
    }
    if ($index == 1) {
        print $fh "None";
        if (defined ($k->{units})) {
           print $fh " (Units = " . $k->{units} . ")";
        }
    }    
    print $fh "</td></tr>";

##### Allowed values block


    my $columnsForValues = 4;
    my $haveIncludeFile = 0;
    my @enums = sort {$a->{enum_value} <=> $b->{enum_value}} @{$k->{enums}};
    my $numEnums = @enums;

    if (($numEnums > 0) || defined($k->{range_min}) || defined($k->{range_max}))
    {
       print $fh <<EOT;
<tr>
 <th colspan=$total_columnspan align=left valign=top>Allowed Values</th>
</tr>
EOT
    }
    if (($k->{type} eq "octet_string") || ($k->{type} eq "unicode_string")) {
       print $fh "<tr><td>Minimum string length</td><td colspan=2>$k->{range_min} octets</td></tr>" if defined $k->{range_min};
       print $fh "<tr><td>Maximum string length</td><td colspan=2>$k->{range_max} octets</td></tr>" if defined $k->{range_max};
    }
    else {
       if (defined $k->{range_min}) {
          if ($k->{'format'} eq "hexadecimal") {
             $k->{'type'} =~ m/uint(\d+)/;
             my $hex_chars = $1/4 . "x";
             $k->{range_min} = sprintf("0x%0$hex_chars", $k->{range_min});
          }
          print $fh "<tr><td>Minimum</td><td colspan=2>$k->{range_min}";
          if ($k->{units} =~ /^[0-9]/) {
            print $fh " x";
          }
          print $fh " $k->{units}</td></tr>";

       }
       if (defined $k->{range_max}) {
          if ($k->{'format'} eq "hexadecimal") {
             $k->{'type'} =~ m/uint(\d+)/;
             my $hex_chars = $1/4 . "x";
             $k->{range_max} = sprintf("0x%0$hex_chars", $k->{range_max});
          }
          print $fh "<tr><td>Maximum</td><td colspan=2>$k->{range_max}";
          if ($k->{units} =~ /^[0-9]/) {
            print $fh " x";
          }
          print $fh " $k->{units}</td></tr>";
       }
    }


    if ($numEnums > 0) {
        my $columnsForEnumDesc = $columnsForValues - 1;
        print $fh <<EOT;
 <tr>
  <th align=left>Defined name</th>
  <th align=left>Value</th>
  <th align=left>Description</th>
 </tr>
EOT

        for (my $i=0; $i < $numEnums; $i++)
        {
            print $fh "<tr><td align=left>";
            print $fh html_quote_block(@enums[$i]->{enum_label});

            print $fh "</td><td align=left>";
            print $fh html_quote_block(@enums[$i]->{enum_value});

            print $fh "</td><td>";
            print $fh html_quote_block(@enums[$i]->{enum_description});

            print $fh "</td></tr>";
        }
    }

##### Main description block

    print $fh "<tr><td colspan=$total_columnspan align=left valign=top><p><b>";
    print $fh html_quote_block($k->{label});
    
    print $fh "</b></p><p>";
    print $fh html_quote_block($k->{description_user});

    print $fh "</p>";

    if (defined($k->{description_internal}) && (defined ($unfiltered))) {
       print $fh "<p><u><i>CSR internal description</i></u></p><p>";
       print $fh html_quote_block($k->{description_internal});
       print $fh "</p>";
    }

    if (defined($k->{source_reference})||defined($k->{source_default})||defined($k->{access_rights})||defined($k->{additional_information})||defined($k->{oid})||($k->{is_internal} =~ /true/i)) {
      print $fh "<p><u><i>Additional Information</i></u></p>";
      
      if (defined($k->{source_reference})) {
         print $fh "<p><i>Source Reference:</i> $k->{source_reference}</p>";
      }
      if (defined($k->{oid})) {
         print $fh "<p><i>OID:</i> $k->{oid}</p>";
      }
      if (defined($k->{source_default})) {
         print $fh "<p><i>Source Default:</i> $k->{source_default}</p>";
      }
      if (defined($k->{access_rights})) {
         print $fh "<p><i>Access Rights:</i> $k->{access_rights}</p>";
      }
      if (defined($k->{additional_information})) {
         print $fh "<p>$k->{additional_information}</p>";
      }
      if ($k->{is_internal} =~ /true/i) {
         print $fh "<p><i><b>FOR CSR USE ONLY</b></i></p>";
      }
    }


##### Functions

    if (exists($k->{functions}[0])) {
       print $fh <<EOT;
<tr>
 <th colspan=$total_columnspan align=left valign=top>Access Functions</th>
</tr>
EOT

       foreach my $function (@{$k->{functions}}) {
       
          print $fh "<tr><td>$function->{type} function</td><td>$function->{function_name}</td>";
          if ($function->{is_for_vif} =~ m/true/i) {
             print $fh "<td>Per-VIF function</td></tr>";
          }
          else {
             print $fh "<td>Global function</td></tr>";
          }
       }

    }

    print $fh "</td></tr></table><br>";
}

sub create_index
{
    my($fh, $title, @ordered_keys) = @_;

    # This function creates an index of HTML hotlinks.
    # Link names have " " replaced with "_" for compatibility

    my $escapedTitle = $title;
    $escapedTitle =~ s/ /_/g;
    print $fh "<h2><a name=\"$escapedTitle\">$title</a></h2>\n";

    print $fh "<table class=\"unboxed\" border=\"0\">\n";

    foreach my $k (@ordered_keys) {
        print $fh
          " <tr><td>$k->{hexnum}</td>",
          "<td><a href=\"#$k->{name}\">$k->{name}</a></td></tr>\n";
    }
    print $fh "</table><br>\n";
}

sub create_title
{
    my($fh, $title, $a, $b) = @_;

    # This function creates the title of the page.
    # Link names have " " replaced with "_" for compatibility

    $a =~ s/ /_/g;
    $b =~ s/ /_/g;

    print $fh <<EOT;
<h1>$title</h1>
<ul>
  <li><a href="#$a">Alphabetical</a></li>
  <li><a href="#$b">Numerical</a></li>
  <li><a href="#permuted_index">Permuted</a></li>
</ul>
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

