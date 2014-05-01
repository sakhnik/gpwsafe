#!/usr/bin/env python

import pexpect, sys, os

gpwsafe = "../src/gpwsafe"
test_file = "/tmp/test.ps3"
password = "asdfqwer"

try:
    os.remove(test_file)
except:
    pass

def Create():
    child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file
                          + ' --create')
    child.expect('Enter passphrase for ' + test_file + ': ')
    child.sendline(password)
    child.expect('Reenter passphrase for ' + test_file + ': ')
    child.sendline(password)
    child.expect('')

try:
    Create()
except Exception as e:
    print("Exception", e)
    sys.exit(1)
