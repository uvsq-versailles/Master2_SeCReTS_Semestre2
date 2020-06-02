#!/usr/bin/python3
# The key found by this program: sxmbimosbdpenullpyjvgwfcotflaplr

import os
import time
import string
import random

# Alphabet for each characters of the key.
alphabet = string.ascii_letters + string.digits + string.punctuation
# Number of possibility in the alphabet.
ALPH_LEN = len(alphabet)

# Lenght of the full key in bytes (256 bits / 8).
KEY_LEN = 32
# The key that we will construct iteratively (initialization to 'a' doesn't matter).
key = list("a" * KEY_LEN)

# Table to store timings for each characters of the alphabet.
timings = [0] * ALPH_LEN

# While we haven't found the correct key (if there is an error of measurement
# for some bytes):
while (True):
    # For each characters of the key.
    for key_it in range(KEY_LEN):
        # For each characters of the alphabet.
        for alph_it in range(ALPH_LEN):
            # Construct the key to test.
            key[key_it] = alphabet[alph_it]
            cmd = "echo \\" + '\\'.join(key) + " | ./dumbsmartcard > /dev/null"
            
            # Perform the profiling during the test.
            ts = time.time_ns()
            os.system(cmd)
            te = time.time_ns()
            timings[alph_it] = te - ts

        # Search the character that produce the slowest execution time.
        timing_min = min(timings)
        for alph_it in range(ALPH_LEN):
            if timings[alph_it] == timing_min:
                key[key_it] = alphabet[alph_it]
                break

        # Temporize the research, because if it's not, the measurements will be wrong
        # (don't know why).
        time.sleep(2)

    # Test the complete found key.
    print('Key : %s' % ''.join(key))
    cmd = "echo \\" + '\\'.join(key) + " | ./dumbsmartcard > /dev/null"
    if (os.system(cmd) >> 8) != 1:
        print("Key found!")
        exit()
