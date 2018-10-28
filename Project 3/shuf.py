#!/usr/bin/env python3

import random, sys, string
from optparse import OptionParser
class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        lines = f.readlines()
        f.close()
        self.lines = [line.strip('\n') for line in lines]

    def chooseline(self):
        return random.choice(self.lines)

def echo (words, repeat, headCount):
    k = 0
    while k < headCount :
        if(len(words) <= 0):
            return
        x = random.randint(0,len(words)-1)
        print(words[x])
        k = k + 1
        if(not repeat):
            words.pop(x)
    return

def printFile(file, repeat, headCount):
    if headCount > len(file) and not repeat:
        headCount = len(file)

    for j in range (headCount):
       x = random.choice(file)
       sys.stdout.write(x)
       
       if(not repeat):
           file.remove(x)
   
def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

    Output randomly selected lines from FILE."""

    input_lines =[]

    parser = OptionParser(version=version_msg,
        usage=usage_msg)

    parser.add_option("-e", "--echo", action="append", dest="echoArray", default=[])

    parser.add_option("-n", "--head-count=COUNT",
        action="store", dest="headCount",
        default=sys.maxsize, help="output at most COUNT lines (default number of lines in file)")

    parser.add_option("-r", "--repeat",
        action="store_true", dest="repeat",
        default=False, help="output lines can be repeated")

    options, args = parser.parse_args(sys.argv[1:])

    try:
        headCount = int(options.headCount)
    except:
        parser.error("invalid NUMLINES: {0}".format(options.headCount))

    try:
        repeat = bool(options.repeat)
    except:
        parser.error("invalid option -- '{0}'".format(options.repeat))

    if headCount < 0:
        parser.error("negative count: {0}".format(options.headCount))

    if headCount == 0:
        return
    
    words = []
    
    if(len(options.echoArray) > 0):
        count = 0
        for i in options.echoArray:
            words.append(i)
        for j in args:
            words.append(j)
        echo(words, repeat, headCount)
        return
    else:
        if len(args) != 0 and args[0] != '-':
            input_file = args[0]
            f = open(input_file, 'r')
            lines = f.readlines()
            f.close()
        else:
            lines = sys.stdin.readlines()    
            
    try:
        printFile(lines, repeat, headCount)

    except IOError as err:
        errno = err.errno
        strerror = err.strerror
        parser.error("I/O error({0}): {1}".format(errno, strerror))

if __name__ == "__main__":
    main()
