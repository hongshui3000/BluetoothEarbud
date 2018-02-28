#!/usr/local/bin/perl -w
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
#
# NAME
#   export_xml.pm  -  Outputs XML metadata for a single subfw version
#

package export_xml;
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

    $opt_out = $output_file;

}

sub start_chapter {
    my ($build_info) = @_;

    print $ofh <<EOT;
<?xml version="1.0" encoding="UTF-8"?>
<metadata_list  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="HydraMeta.xsd">
EOT
    
    print $ofh "<metadata  subsystem_name=\"$build_info->{subsystem_name}\" ";
    print $ofh "subsystem_layer=\"$build_info->{subsystem_layer}\" " if defined $build_info->{subsystem_layer};
    print $ofh "subsystem_alias=\"$build_info->{subsystem_alias}\" " if defined $build_info->{subsystem_alias};
    print $ofh "subsystem_id=\"$build_info->{subsystem_id}\" " if defined $build_info->{subsystem_id};
    print $ofh "version=\"$build_info->{version}\" " if defined $build_info->{version};
    print $ofh "variant=\"$build_info->{variant}\" " if defined $build_info->{variant};
    print $ofh "description=\"$build_info->{description}\" " if defined $build_info->{description};
    print $ofh "build_date_time=\"$build_info->{build_date_time}\" " if defined $build_info->{build_date_time};
    print $ofh ">\n\n";
}


sub add_enum {
   my ($enum_name, @enum_values) = @_;
   print $ofh "<enum_def enum_name=\"$enum_name\">\n";
   
   foreach my $value (@enum_values) {
      print $ofh "  <enum_entry";
      
      my @tags = qw( enum_label enum_value enum_description );
      foreach my $tag (@tags) {
         print $ofh " $tag=\"$value->{$tag}\"" if defined $value->{$tag};
      }
      print $ofh "/>\n";

   }
   print $ofh "</enum_def>\n\n";
}

sub add_struct {
   my ($struct_name, $struct_elements, $unfiltered) = @_;
   print $ofh "<struct_def struct_name=\"$struct_name\">\n";
   
   foreach my $elem (@$struct_elements) {
      print $ofh "  <structure_element";
      
      my @tags = qw( lsb_bit_position bit_width );
      foreach my $tag (@tags) {
         print $ofh " $tag=\"$elem->{$tag}\"" if defined $elem->{$tag};
      }
      print $ofh " name=\"$elem->{element}->{name}\">\n";
      
      # general fields
      my @tags = qw( label type format range_min range_max array_length_min array_length_max );
      foreach my $tag (@tags) {
         print $ofh "    <$tag>$elem->{element}->{$tag}</$tag>\n" if defined $elem->{element}->{$tag};
      }
      
      my @tags = qw( description_user description_internal );
      
      # description fields
      foreach my $tag (@tags) {
         if (defined($unfiltered) || ($tag ne "description_internal")) {
            $elem->{element}->{$tag} =~ s/\n$//;
            if ($elem->{element}->{$tag} =~ /<[a-zA-Z0-9 \/]+>/) {
               print $ofh "    <$tag><![CDATA[$elem->{element}->{$tag}]]></$tag>\n" if defined $elem->{element}->{$tag};
            }
            else {
               # Escape any special chars
               $elem->{element}->{$tag} =~ s/&/&amp;/g;
               $elem->{element}->{$tag} =~ s/\"/&quot;/g;
               $elem->{element}->{$tag} =~ s/</&lt;/g;
               $elem->{element}->{$tag} =~ s/>/&gt;/g;

               print $ofh "    <$tag>\n$elem->{element}->{$tag}\n    </$tag>\n" if defined $elem->{element}->{$tag};
            }
         }
      }
      print $ofh "  </structure_element>\n\n";
   }
   print $ofh "</struct_def>\n\n";
}


sub add_config_element {
   my ($elem, $unfiltered) = @_;
   if (defined($unfiltered) || ($elem->{is_internal} !~ /true/i)) {
      print $ofh "<config_element name=\"$elem->{name}\" psid=\"$elem->{psid}\">\n";
      
      # general fields
      my @tags = qw( label type format range_min range_max oid access_rights units source_reference source_default category table_name array_length_min array_length_max );
      foreach my $tag (@tags) {
         print $ofh "  <$tag>$elem->{$tag}</$tag>\n" if defined $elem->{$tag};
      }
      
      my @tags = qw( description_user description_internal additional_information );
      
      # description fields
      foreach my $tag (@tags) {
         if (defined($unfiltered) || ($tag ne "description_internal")) {
            $elem->{$tag} =~ s/\n$//;
            if ($elem->{$tag} =~ /<[a-zA-Z0-9 \/]+>/) {
               print $ofh "  <$tag><![CDATA[$elem->{$tag}]]></$tag>\n" if defined $elem->{$tag};
            }
            else {
               # Escape any special chars
               $elem->{$tag} =~ s/&/&amp;/g;
               $elem->{$tag} =~ s/\"/&quot;/g;
               $elem->{$tag} =~ s/</&lt;/g;
               $elem->{$tag} =~ s/>/&gt;/g;

               print $ofh "  <$tag>\n$elem->{$tag}\n  </$tag>\n" if defined $elem->{$tag};
            }
         }
      }

      # default array
      if (exists($elem->{default}[0])) {
         print $ofh "  <default_list>\n" if (exists($elem->{default}[1]));

         foreach my $default ( sort {$a->{index1_value} <=> $b->{index1_value} ||
                                     $a->{index2_value} <=> $b->{index2_value} ||
                                     $a->{index3_value} <=> $b->{index3_value}   } @{$elem->{default} }) {
             print $ofh "  " if (exists($elem->{default}[1]));
             print $ofh "  <default";
             print $ofh " index1=\"$default->{index1_value}\"" if defined $default->{index1_value};
             print $ofh " index2=\"$default->{index2_value}\"" if defined $default->{index2_value};
             print $ofh " index3=\"$default->{index3_value}\"" if defined $default->{index3_value};

             $default->{string} =~ s/\\<br>//g;
             if ($default->{string} =~ /<[a-zA-Z0-9 \/]+>/) {
                print $ofh "><![CDATA[$default->{string}]]></default>\n";
             }
             else {
                # Escape any special chars
                $default->{string} =~ s/&/&amp;/g;
                $default->{string} =~ s/\"/&quot;/g;
                $default->{string} =~ s/</&lt;/g;
                $default->{string} =~ s/>/&gt;/g;

                print $ofh ">$default->{string}</default>\n";
             }
             
         }

         print $ofh "  </default_list>\n" if (exists($elem->{default}[1]));
      }
      
      # functions array
      if (exists($elem->{functions}[0])) {
         print $ofh "  <function_list>\n" if (exists($elem->{functions}[1]));
         foreach my $function (@{$elem->{functions}}) {
            print $ofh "  " if (exists($elem->{functions}[1]));
            print $ofh "  <function function_name=\"$function->{function_name}\" type=\"$function->{type}\" is_for_vif=\"$function->{is_for_vif}\"/>\n";
         }
         print $ofh "  </function_list>\n" if (exists($elem->{functions}[1]));
      }

      @tags = qw( is_array is_internal );
      foreach my $tag (@tags) {
         print $ofh "  <$tag>$elem->{$tag}</$tag>\n" if ((defined $elem->{$tag}) && ($elem->{$tag} ne "0"));
      }
      
      print $ofh "</config_element>\n\n";
   }
}

sub add_config_table {
   my ($table, $unfiltered) = @_;
   if (defined($unfiltered) || ($table->{is_internal} !~ /true/i)) {
      print $ofh "<config_table table_name=\"$table->{table_name}\" num_indices=\"$table->{num_indices}\">\n";
      
      # index fields
      my @tags = qw( index1 index2 index3 );
      foreach my $tag (@tags) {
         print $ofh "  <$tag name=\"$table->{$tag}\"/>\n" if defined $table->{$tag};
      }
      
      # general fields
      my @tags = qw( oid source_reference );
      foreach my $tag (@tags) {
         print $ofh "  <$tag>$table->{$tag}</$tag>\n" if defined $table->{$tag};
      }
      
      my @tags = qw( description_user description_internal );
      
      # description fields
      foreach my $tag (@tags) {
         if (defined($unfiltered) || ($tag ne "description_internal")) {
            $table->{$tag} =~ s/\n$//;
            if ($table->{$tag} =~ /<[a-zA-Z0-9 \/]+>/) {
               print $ofh "  <$tag><![CDATA[$table->{$tag}]]></$tag>\n" if defined $table->{$tag};
            }
            else {
               # Escape any special chars
               $table->{$tag} =~ s/&/&amp;/g;
               $table->{$tag} =~ s/\"/&quot;/g;
               $table->{$tag} =~ s/</&lt;/g;
               $table->{$tag} =~ s/>/&gt;/g;

               print $ofh "  <$tag>\n$table->{$tag}\n  </$tag>\n" if defined $table->{$tag};
            }
         }
      }

      @tags = qw( is_internal );
      foreach my $tag (@tags) {
         print $ofh "  <$tag>$table->{$tag}</$tag>\n" if ((defined $table->{$tag}) && ($table->{$tag} ne "0"));
      }
      
      print $ofh "</config_table>\n\n";
   }
}


sub add_chapter {
    my ($unfiltered) = @_;
}

sub close_doc {
   print $ofh "</metadata>\n</metadata_list>\n";
   
   # Close the output file
   close($ofh);
   print "Created $opt_out\n";
}




# return true to use statement that includes this module
1;

