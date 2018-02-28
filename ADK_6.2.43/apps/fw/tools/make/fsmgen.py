#!/usr/bin/python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

import os, sys, re, optparse

''' FILE
       fsmgen.py  -

 FUNCTION

       Take the state machine file and convert into definition and
       constant header files.
'''

'''
use vars qw($state_max $event_max $action_max
            $state_len $event_len $action_len
            %states @states %events @events %actions @actions
            %fsm_files $template_file $file $lmp_def_file
            %fsm_vars %subs $forcount @forlist @markerstack %markers
            $skip_empty $opt_c $opt_p $opt_v $opt_w
            $fsm_event_base $commentstart $commentend
            %offsets $offset_len)
'''

def evaluate_pp_conditional(line, defs):
    '''
    Evaluates a #ifdef or #ifndef.  #if ... is too hard to worry about
    until it actually appears.
    '''
    # First the easy cases - '#if(n)def CONDITIONAL'
    if line[0:3] == "def":
        return line.split()[1].strip() in defs
    if line[0:4] == "ndef":
        return not line.split()[1].strip() in defs
    
    raise ValueError("Can't evaluate complex conditionals yet :(")

def preprocess(fh, defs):
    '''
    Returns a list of lines from filehandle 'fh' after considering the
    macro definitions given 
    '''
    pplines = []
    printing_stack = [True]
    for line in fh:
        if re.match(r"^#.*$", line):
            if re.match(r"^#if", line):
                # If we're currently printing lines, evaluate the condition
                # (which has to be done recursively), and push the result onto 
                # the printing stack.  If we're not printing, there's no need to
                # evaluate it.
                if printing_stack[-1]:
                    printing_stack.append(evaluate_pp_conditional(line[3:], defs))
                else:
                    printing_stack.append(False)
                
            elif re.match(r"^#else.*$", line):
                printing_stack[-1] = not printing_stack[-1]
                
            elif re.match(r"^#endif.*$", line):
                printing_stack.pop()
                
        elif printing_stack[-1]:
            pplines.append(line)
            
    return pplines

# Remove $s, @s change empty list+dict syntax
state_max = 0
state_len = 0
event_max = 0
event_len = 0
action_max = 0
action_len = 0
offset_len = 0
states = {}
events = {}
actions = {}
fsm_vars = {}
fsm_files = {}
opt_c = 0
opt_p = 1
opt_v = 0
opt_w = 0
fsm_event_base = 0
skip_empty = 0
offsets = {}
defs = {}

if len(sys.argv) < 3:
    print "Usage: %s <.fsm file> <build dir> [-D<CONDITIONAL> ...]"
    sys.exit(1)
    
infile = sys.argv[1]
if infile:
    if not  os.path.exists(infile) and os.path.exists("%s.fsm" % infile):
        infile += ".fsm"
    if not  os.path.exists(infile):
        print "Usage: sys.argv[0] <.fsm file>"
        sys.exit(1)
        
fsm_dir = os.path.dirname(infile)
fsm_output_dir = "/".join([sys.argv[2], fsm_dir]) 

for df in sys.argv[3:]:
    if "=" in df:
        name,val = df.split("=")
    else:
        name,val = df,1
    defs[name] = val

commentstyle = None
    
#
# Scan the input .csv file.
#

try:
    FILE = open(infile)
except IOError as e:
    print "Can't read %s: %s" % (infile, e)
    sys.exit(1)

#"Pre-process" the file
FILEPP = preprocess(FILE, defs)

#Extract FSM name
_ , fsm_name = os.path.split(infile)
fsm_name, _ = fsm_name.split(".")

ppdump = open("%s/ppfile_%s.out" % (fsm_output_dir, fsm_name), "w")
for line in FILEPP:
    ppdump.write(line)
ppdump.close()

read_first_line = False
for fullline in FILEPP:
    cur = ''

    # Skip comment lines
    if re.search(r"^\#", fullline) or re.search(r"-----", fullline):
        continue

    # Don't want the <any other event>
    if re.search(r"<any other event>", fullline):
        continue

    # Strip the input line into its seperate parts and push them
    # individually ont line. Hence will give,  line[0]=state,
    # line[1]=event etc..

    # Don't want to split lines starting with "__FSM"
    if re.search(r"__FSM", fullline):
        line = [fullline]
    else:
        # Otherwise, split the line into words
        line = fullline.split()
    # Don't want "<any other event>" or stuff following it
    try:
        line = line[:line.index("<any other event>")]
    except ValueError:
        pass
        
    # The first line is special and we expect 'State'
    if not read_first_line:
        if line[0] != 'State':
            sys.stderr.write("fsmgen.py WARNING: unexpected header at %s line 1, was '%s'. Continuing.\n" % (infile, line[0]))
        read_first_line = True
    elif not line or not line[0]:
        continue
    elif line[0].startswith("_"):
        if re.search(r"^__FSM_COMMENTS_C", line[0]):
            commentstyle = "C"
            if opt_v:
                print "Using C-style comments"
        elif re.search(r"^__FSM_COMMENTS_XAP", line[0]):
            commentstyle = "XAP"
            if opt_v:
                print "Using XAP-style comments" 
        elif re.search(r"^__FSM_(START|END|SRC_LIST|COMMENTS)", line[0]):
            if opt_v:
                print "Ignoring %s" % line[0]  
        else:
            match = re.search(r"__FSM_VAR\(\s*(\S+)\s*,\s*(\S+)\)", line[0])
            if match:
                fsm_vars[match.group(1).lower()] = match.group(2)
                if opt_v:
                    print "Setting FSM variable '%s' to '%s'" % (match.group(1), 
                                                                  match.group(2))
                continue
        
            match = re.search(r"__FSM_TEMPLATE_FILE\((.*)\)", line[0])
            if match:
                template_file = match.group(1)
                template_file = re.sub(r"\\",r"/", template_file)
                # Need this for Apps tree structure/build dir etc.
                template_file_path = os.path.join(fsm_dir, template_file)
                if opt_v:
                    print "Setting FSM template file %s\n" % template_file
                    if not os.path.exists(template_file_path):
                        print "fsmgen.py WARNING: template file does not exist: %s at %s. continuing" % (template_file, infile)
                continue
            
            match = re.search(r"__FSM_OUTPUT_FILE\((\S+),\s*(\S+),\s*(\S+),\s*(\S+)\)", line[0])
            if match:
                fsm_files[match.group(3)] = [ template_file, 
                                             match.group(1), 
                                             match.group(2), match.group(4), 
                                             commentstyle ]
                if opt_v:
                    print """Setting FSM files for: %s to: 
 "%s","%s","%s","%s","%s" """ % (match.group(3),)+tuple(fsm_files[match.group(3)])
                continue
  
            match = re.search(r"__FSM_LMP_DEF_FILE\((\S+)\)", line[0])
            if match:
                print "ERROR: fsmgen.py doesn't support LMP_DEF_FILE!"
                sys.exit(1)

            # If no matches
            print "**%s**" % line[0]
            print "fsmgen.py WARNING: skipping FSM information: %s. Continuing" % line[0]
    else:
        if not line[0] in states:
            state_max += 1
            if len(line[0]) > state_len:
                state_len = len(line[0])  
            states[line[0]] = {}

        # This holds the state information i.e
        # current_state, event, next_state, action, comment stored in the
        # hash as state{curstate}{event} hence will look something like
        #    {S1} {E1} = [NS1, A1, comment]
        #    {S1} {E2} = [NS2, A2, comment]
        #    etc..
        # states{curstate}{event} = [ nextstate, action, comment ]
        if line[1]:
          states[line[0]][line[1]] = [ line[2], line[3], 
                                      line[4]  if len(line) == 5 else None ]

        if line[1] and line[1] not in events:
            if re.match("^\s*(\<.*\>)\s*$", line[1]):
                continue
            # The event is stored in a hash as event_name,number.
            events[line[1]] = event_max
            event_max += 1
            if len(line[1]) > event_len:
                event_len = len(line[1]) 

        if line[3] and line[3] not in actions:
            if line[3] == 'NULL': # handled specially            
                continue 
            actions[line[3]] = action_max
            action_max += 1
            if len(line[3]) > action_len:
                action_len = len(line[3])  

FILE.close()

bad = 0
for state, sref in states.iteritems():
    for event, eref in sref.iteritems():
        if eref[0] not in states:
            print "Transition from state '%s' to non-existent state '%s' on event '%s'" \
                % (state, event, eref[0])
            bad += 1
if bad:
    sys.exit(1)

# for recursive use

def handle_for (fortype_and_lines, NEWFILE):
    
    global skip_empty
    
    fortype = fortype_and_lines[0]
    lines = fortype_and_lines[1:]
    i = 0

    fortype.strip()

    if re.match("__FSM_FOR_STATES", fortype):
        for item in states.keys():
            subs["__FSM_STATE_NAME"] = item
            subs["__FSM_STATE_NAME_UPPER"] = item.upper()
            subs["__FSM_STATE_NAME_LOWER"] = item.lower()
            subs["__FSM_STATE_NO"] = i
            i += 1
            subs["__FSM_PAD_STATE"] = ' '*(state_len - len(item))
            # I don't know what @_ refers to here!
            for line in lines:
                handle_line(line, NEWFILE)

        del subs["__FSM_STATE_NAME"]
        del subs["__FSM_STATE_NAME_UPPER"]
        del subs["__FSM_STATE_NAME_LOWER"]
        del subs["__FSM_STATE_NO"]
        del subs["__FSM_PAD_STATE"]
        
        return
        
    match = re.match("__FSM_FOR_EVENTS(_IN_CLASS\((.*)\))?", fortype)
    if match:
        if match.groups() == 2 and match.group(1) and match.group(2):
            return  # don't know what to do yet.
        for item in events.keys():
            try:
                state = subs["__FSM_STATE_NAME"]
                stateref = states[state][item]
                nextstate = stateref[0]
                action = stateref[1]
                subs["__FSM_NEXT_STATE"] = nextstate
                if action is not None:
                    subs["__FSM_ACTION_NAME"] = action
                    subs["__FSM_ACTION_NAME_UPPER"] = action.upper()
                    subs["__FSM_ACTION_NAME_LOWER"] = action.lower()
                    # subs["__FSM_ACTION_NO"] = actions{action}
                    subs["__FSM_PAD_ACTION"] =\
                      ' '*(action_len - len(action))
                    if opt_c and stateref[2] is not None:
                        print "    /* %s */" % stateref[2]
            except KeyError:
                pass
            skip_empty = 0

            subs["__FSM_EVENT_NAME"] = item
            subs["__FSM_EVENT_NAME_UPPER"] = item.upper()
            subs["__FSM_EVENT_NAME_LOWER"] = item.lower()
            subs["__FSM_EVENT_NO"] = events[item]
            subs["__FSM_PAD_EVENT"] = ' '*(event_len - len(item))
            for line in lines:
                handle_line(line, NEWFILE)
            try:
                del subs["__FSM_NEXT_STATE"]
                del subs["__FSM_ACTION_NAME"]
                del subs["__FSM_ACTION_NAME_UPPER"]
                del subs["__FSM_ACTION_NAME_LOWER"]
                # del subs["__FSM_ACTION_NO"]
                del subs["__FSM_PAD_ACTION"]
            except KeyError:
                # Maybe there is no action
                pass

        skip_empty = 0
        del subs["__FSM_EVENT_NAME"]
        del subs["__FSM_EVENT_NAME_UPPER"]
        del subs["__FSM_EVENT_NAME_LOWER"]
        del subs["__FSM_EVENT_NO"]
        del subs["__FSM_PAD_EVENT"]
        
        return
        
    match = re.match("__FSM_FOR_OFFSETS", fortype)
    if match:
        for item in sorted(offsets.keys()):
            subs["__FSM_OFFSET_NAME"] = item
            subs["__FSM_OFFSET_NAME_UPPER"] = item.upper()
            subs["__FSM_OFFSET_NAME_LOWER"] = item.lower()
            subs["__FSM_OFFSET_NO"] = offsets[item]
            subs["__FSM_PAD_OFFSET"] = ' '*(offset_len - len(item))
            for line in lines:
                handle_line(line, NEWFILE)

        del subs["__FSM_OFFSET_NAME"]
        del subs["__FSM_OFFSET_NAME_UPPER"]
        del subs["__FSM_OFFSET_NAME_LOWER"]
        del subs["__FSM_OFFSET_NO"]
        del subs["__FSM_PAD_OFFSET"]
        
        return
        
        # Generate Debugging Event Strings
    match = re.match("__FSM_FOR_EVENT_LOG_STRING_DECLS", fortype)
    if match:
        last = 0
        for item in sorted(events.keys(), cmp=lambda a, b: events[a]-events[b]):

            # Update where we got to
            last = events[item]
            skip_empty = 0
            subs["__FSM_LOG_EVENT_NAME_UPPER"] = item.upper()
            for line in lines:
              handle_line(line, NEWFILE)

        skip_empty = 0
        del subs["__FSM_LOG_EVENT_NAME_UPPER"]
        return
        
    match = re.match("__FSM_FOR_EVENT_LOGS", fortype)
    if match:
        last = 0
        # Sort Events into numeric order
        for item in events.keys():
            if item is None:
                print "Key value undefined in events"

            if item not in events or events[item] is None:
                print "No value for event %s" % item

        for item in sorted(events.keys(), cmp=lambda a, b: events[a]-events[b]):
            # Update where we got to
            last = events[item]
            skip_empty = 0
            subs["__FSM_LOG_EVENT_NAME_UPPER"] = item.upper()
            for line in lines:
              handle_line(line, NEWFILE)
  
        skip_empty = 0
        del subs["__FSM_LOG_EVENT_NAME_UPPER"]
        return
        
    match = re.match("__FSM_FOR_ACTIONS", fortype)
    if match:
        for item in actions.keys():
            subs["__FSM_ACTION_NAME"] = item
            subs["__FSM_ACTION_NAME_UPPER"] = item.upper()
            if opt_p:
                subs["__FSM_ACTION_NAME_LOWER"] = item
            else:
                subs["__FSM_ACTION_NAME_LOWER"] = item.lower()

            subs["__FSM_ACTION_NO"] = i
            i += 1
            subs["__FSM_PAD_ACTION"] = ' '*(action_len - len(item))
            for line in lines:
                handle_line(line, NEWFILE)

        del subs["__FSM_ACTION_NAME"]
        del subs["__FSM_ACTION_NAME_UPPER"]
        del subs["__FSM_ACTION_NAME_LOWER"]
        del subs["__FSM_ACTION_NO"]
        del subs["__FSM_PAD_ACTION"]
        
        return
    
    print "For loop %s not handled" % fortype
    sys.exit(1)

forlist = []
markerstack = []
forcount = 0

def unescape(esc):

    match = re.match("([0-7])([0-7])([0-7])", esc)
    if match:
        return chr(match.group(1)*64 + match.group(2)*8 + match.group(3))
    match = re.match("x([0-9a-fA-F][0-9a-fA-F])", esc)
    if match:
        return chr(hex(match.group(1)))
    if esc == "n": 
        return "\n" 
    if esc == "r":
        return "\r"
    if esc == "t":
        return "\t"
    print "Unknown escape sequence '%s' at $file line $." % esc
    sys.exit(1)


def handle_line (line, NEWFILE):

    global skip_empty, forcount, forlist, markerstack,commentstart,commentend
    
    if skip_empty and "__FSM_NEXT_STATE" not in subs:
        return True
    
    if markerstack:
        # Set when we have handled a marker from the template file
        # and need to dump the text up to the end marker in the
        # current file.
        match = re.search(r"(.*__FSM_END_MARKER)(.*)", line)
        if match:
            intro = match.group(1)
            if not re.search(r"/\*.*\*/", line):
                line = "%s}}%s(%s, %s %s\n" % \
                 (commentstart, intro, subs["__FSM_NAME_UPPER"], markerstack[0], commentend)
            NEWFILE.write(line)
            markerstack.pop()
        return True
        
    match = re.search(r"^__FSM_FOR_", line)
    if match:
        forcount += 1
        forlist.append(line)
        return True
    
    if forcount:
        # Set when we are reading a for loop and looking for the
        # end line.
        if re.search(r"^__FSM_END_FOR$", line):
            forcount -= 1

        if forcount == 0:
            # Careful in case we need to handle nested for loops.
            oldlist = forlist[:]
            forlist = []
            handle_for(oldlist, NEWFILE)
        else:
            forlist.append(line)
        
        return True
    
    elif re.search(r"^__FSM_END_FOR", line):
        print "Unhandled FSM_END_FOR:\n$_  at $file line $.,\n  fatal.\n"
        sys.exit(1)
    
    match = re.search(r"(.*__FSM_MARKER\()((?:|.*\s*,\s*)(\S[^,]+)\))", line)
    if match:
        intro = match.group(1)
        trailer = match.group(2)
        typ = match.group(3)
        if not re.search(r"/\*.*\*/", line):
            line = "%s{{%s%s%s%s\n" % (commentstart,intro,
                                       subs["__FSM_NAME_UPPER"], trailer,
                                       commentend)
        NEWFILE.write(line)
        try:
            for lline in markers[typ]:
                handle_line(lline, NEWFILE)
        except KeyError:
            pass

        markerstack.append(trailer)
        return True
        
    match = re.search(r"__FSM_(|END_)MARKER", line)
    if match:
        print "Unhandled FSM marker:\n$_  at $file line $.,\n  fatal.\n"
        sys.exit(1)
      
    match = re.search(r"^__FSM_END$", line)  
    if match:
        return None
    
    match = re.search(r"^__FSM_SKIP_EMPTY_TRANSITIONS$", line)
    if match:
        skip_empty = 1
    else:

        # Do longest substitution first.
        # This can be made more efficent by burying %subs in
        # wrappers.
        for key in sorted(subs.keys(), cmp=lambda a,b : len(b)-len(a)):
            if key is None:
                print "undefined substitution key"
                sys.exit(1)
            try:
                val = subs[key]
            except KeyError:
                print "undefined substitution for $s" % key
                
            line = re.sub(key,str(val),line)
        
        line = re.sub(r"__FSM_NO_NEWLINE[\r\n]*$","",line)
        line = re.sub(r"__FSM_VAR\(([^\)]+)\)",
                      lambda match: fsm_vars[match.group(1).lower()],
                      line)
        
        match = re.search(r"^__FSM_SET_VAR\(\s*(\S+)\s*,\s*(.*?)\s*\)$", line)
        if match:
            var, val = match.group(1), match.group(2)
            var = var.lower()
            
            val = re.sub(r'^"(.*)"$', lambda match: match.group(1), val)
            if val:
                val = re.sub(r"\\([0-7][0-7][0-7]|x[0-9A-Fa-f][0-9A-Fa-f]|.)",
                             lambda match: unescape(match.group(1)), val)

            fsm_vars[var] = val

            return True
        
        match = re.search(r"^__FSM_INC_VAR\(\s*(\S+)\s*\)$", line)
        if match:
            key = match.group(1).lower()
            if key not in fsm_vars or fsm_vars[key] is None:
                print "Unknown FSM variable %s at $file line $.,\nfatal." % match.group(1)
                sys.exit(1)
            fsm_vars[key] = str(int(fsm_vars[key]) + 1)
            return True
        
        NEWFILE.write(line)

    return True



subs = { "__FSM_TOOL_NAME" : "fsmgen.pl",
          "__FSM_TOOL_VERSION" : "2D",
          "__FSM_NUM_STATES" : state_max,
          "__FSM_NUM_EVENTS" : event_max,
          "__FSM_NUM_ACTIONS" : action_max}


# no obvious way of getting this...
name = os.path.splitext(os.path.basename(infile))[0]
match = re.search(r"(.*)_fsm", name)
if match:
    name = match.group(1)
else:
    name = fsm_vars[stateprefix]
    name = name.rstrip("_ST_")

subs["__FSM_NAME"] = name.upper()
subs["__FSM_NAME_UPPER"] = name.upper()
subs["__FSM_NAME_LOWER"] = name.lower()

for filetype, fileref in fsm_files.iteritems():

    template = 0
    file = fileref[1] + fileref[3] + fileref[2]

    tempfile = fileref[0]
    if tempfile == "/dev/null":
        continue
    
    commentstyle = fileref[4]

    if commentstyle is None:
        print "Comment style not set for file %s" % file
        sys.exit(1)
    elif commentstyle == "C":
        commentstart = '/*'
        commentend = '*/'
    elif commentstyle == "XAP":
        commentstart = ''
        commentend = ''
    else:
        print "Comment style not set for file %s" % file
        sys.exit(1)

    # Scan the template file for markers, whether we are going
    # to be using it for output or not.  These bits need substituting
    # in the target file even if it already exists.
    markers = {}
    try:
        tempfile_path = os.path.join(fsm_dir,tempfile)
        FILE = open(tempfile_path)
    except IOError as e:
        print "Can't read template file %s: %s" % (tempfile_path, e)
        sys.exit(1)
        
    found_marker = False
    for line in FILE:
        line = re.sub("\r\n","\n", line)
        if not found_marker:
            match = re.search(r"__FSM_MARKER\((\S+)\)", line)
            if match:
                cur = match.group(1)
                lines = []
                found_marker = True
        else:
            if re.search(r"__FSM_END_MARKER", line):
                found_marker = False
                markers[cur] = lines
            else:
                lines.append(line)
        
    FILE.close()

    subs["__FSM_OUTPUT_FILE_UPPER"] = fileref[1].upper()
    subs["__FSM_OUTPUT_FILE_LOWER"] = fileref[1].lower()


    file = os.path.join(fsm_output_dir, file)
    try:
        NEWFILE = open(file, "w")
    except IOError as e:
        print "Can't write to %s: %s" % (file, e)
        sys.exit(1)

    print "Writing %s" % file

    if not os.path.exists("%s.old" % file):
      # Use template file.
      file = os.path.join(fsm_dir,fileref[0])
      template = True
    else:
      file = "%s.old" % file

    try:
        FILE = open(file)
    except IOError as e:
        print "Can't read %s.old: %s" % (file, e)
        sys.exit(1)

    if template:
        for line in FILE:
            line = re.sub("\r\n","\n", line)
            if re.match(r"^__FSM_START$", line):
                break

    for line in FILE:
        if template:
            line = re.sub("\r\n","\n", line)
        if not handle_line(line, NEWFILE):
            break

    FILE.close()
    NEWFILE.close()


