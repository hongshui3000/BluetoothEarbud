#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
'''
DESCRIPTION
  Create HTML documentation for API.

  Usage:
      apidocgen.py <input.xml> <output.html>

'''
import sys
import re
try:
    import xml.etree.ElementTree as ET
except ImportError:
    print "\nError importing ElementTree module needed for XML parsing."
    print "Default version of Python needs to be at least 2.6"
    print "Hint: confirm the location and add to PATH in your profile"
    raise

xmlfile = sys.argv[1]
htmlfile = sys.argv[2]

print "Creating HTML documentation from API xml"

#-----------------------------------------------
# Helper functions...
#-----------------------------------------------
def tagprint(h, tag, label, output):
    hNew = ET.SubElement(h, tag)
    hLabel = ET.SubElement(hNew, "span")
    hLabel.text = label + ": "
    hLabel.attrib["class"] = "label"
    
    hOutput = ET.SubElement(hNew, "span")
    hOutput.text = output
    hOutput.attrib["class"] = "output"
     
def celltext(h,text,columns):
    hCell  = ET.SubElement(h, "td")
    hCell.attrib["colspan"] = str(columns)
    if text is not None:
        hCell.text = des.text
    else:
        hCell.text = "-"     
        
def indent(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i        
#-----------------------------------------------
# Load and parse the XML input file...
#----------------------------------------------- 
root = ET.parse(xmlfile).getroot()

#-----------------------------------------------  
# Create the HTML output tree
#-----------------------------------------------  
hHtml = ET.Element("html")
hHead = ET.SubElement(hHtml, "head")

hTitle = ET.SubElement(hHead, "title")
hTitle.text = "API Specification"

hStyle = ET.SubElement(hHead, "style")
hStyle.text = \
"table { border: 2px solid #000099; border-collapse:collapse; width: 900px} \n \
td { border: 1px solid #0000bb; border-collapse:collapse; } \n \
span.label { color: #000099;  font-size: 10pt;} \n \
span.output { color: #000066; font-style: italic;}"

hBody  = ET.SubElement(hHtml, "body")
hHeading  = ET.SubElement(hBody, "h1")
hHeading.text = "API Specification"
tagprint(hBody, "b", "Version", root.find("traps").get("version"))
#-----------------------------------------------  
# Output the traps
#-----------------------------------------------  
hHeading = ET.SubElement(hBody, "h2")
hHeading.text = "Traps"

for trap in root.getiterator("trap"):
    hTrap = ET.SubElement(hBody, "h3")
    hTrap.text = trap.get("name")
    
    hTrap = ET.SubElement(hBody, "p")
    des = trap.find("document").find("description")
    if des.text is not None:
        hTrap.text = des.text
    
    hTable = ET.SubElement(hBody, "table")
    hRow = ET.SubElement(hTable, "tr")
    
    tagprint(hRow, "td", "Trapset", trap.get("trapset"))
    tagprint(hRow, "td", "Header", trap.get("header"))
    tagprint(hRow, "td", "Processor", trap.get("processor"))
    tagprint(hRow, "td", "Lifestage", trap.get("lifestage"))
    
    #Find the traps return value... 
    ret = trap.find("return")
    
    hRow = ET.SubElement(hTable, "tr")
    
    tagprint(hRow, "td", "Returns", "")
    tagprint(hRow, "td", "Type", ret.get("type"))
   
    des = ret.find("document").find("description")
    celltext(hRow,des.text,2)
        
    #Find the traps parameters...     
    params = trap.find("params").getchildren()
    
    if params is not None:
        for param in params:
            hRow = ET.SubElement(hTable, "tr")
            tagprint(hRow, "td", "Parameter", param.get("name"))
            tagprint(hRow, "td", "Type", param.get("type"))
            des = param.find("document").find("description")
            celltext(hRow,des.text,2)    
    else:
        hRow = ET.SubElement(hTable, "tr")
        tagprint(hRow, "td", "Parameters", "none")
        hCell = ET.SubElement(hRow, "td")
        hCell.attrib["colspan"] = "3"
        hCell.text = "-"
        
    hRow = ET.SubElement(hTable, "tr")
    des = trap.find("document").find("documentation")
    celltext(hRow,des.text,4)


#-----------------------------------------------  
# Output the messages. 
#-----------------------------------------------  
hHeading = ET.SubElement(hBody, "h2")
hHeading.text = "Messages"
tagprint(hBody, "b", "Base Index", root.find("messages").get("base"))
for msg in root.getiterator("message"):
    hMsg = ET.SubElement(hBody, "h3")
    hMsg.text = msg.get("name")

    hTable = ET.SubElement(hBody, "table")
    hRow = ET.SubElement(hTable, "tr")
    
    tagprint(hRow, "td", "Index", msg.get("msgidx"))

    #Find the messages parameters...     
    params = msg.find("params")
    
    if params is not None:
        for param in params.getchildren():
            hRow = ET.SubElement(hTable, "tr")
            tagprint(hRow, "td", "parameter", param.get("name"))
            tagprint(hRow, "td", "type", param.get("type"))
            
            des = param.find("document").find("description")
            celltext(hRow,des.text,2)    
    else:
        hRow = ET.SubElement(hTable, "tr")
        tagprint(hRow, "td", "Parameters", "none")
        hCell = ET.SubElement(hRow, "td")
        hCell.attrib["colspan"] = "3"
        hCell.text = "-"
        
    hRow = ET.SubElement(hTable, "tr")
    
    des = msg.find("document").find("documentation")
    celltext(hRow,des.text,4)    
 
#-----------------------------------------------  
# Save the output to a readable HTML file 
#-----------------------------------------------   
indent(hHtml)
ET.ElementTree(hHtml).write(htmlfile)   
