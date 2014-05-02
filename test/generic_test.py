#!/usr/bin/env python

import pexpect, sys, os

gpwsafe = "../src/gpwsafe"
test_file = "/tmp/test.ps3"
password = "asdfqwer"

records = [
    {
        'add_req':   'bar.foo',
        'name':      'foo',
        'group':     'bar',
        'username':  'foo@bar.baz',
        'password':  'Qwer!234',
        'notes':     'First account',

        'query_req': 'bar.foo'
    },
    {
        'add_req':   'baz',
        'name':      'baz',
        'group':     'bar',
        'username':  'foo@bar.baz',
        'password':  'ZxcvAsdf',
        'notes':     'Second account',

        'query_req': 'bar.baz'
    }
]

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
    child.expect(pexpect.EOF)

def Populate():
    for params in records:
        print("Adding " + params['add_req'])
        child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file
                              + ' --add ' + params['add_req'])
        child.setecho(False)
        child.expect('Enter password for ' + test_file + ': ')
        child.sendline(password)
        while True:
            patterns = ['.*group \[<none>\]: ',
                         '.*username: ',
                         '.*password \[return for random\]: ',
                         '.*password again: ',
                         '.*notes: ',
                         pexpect.EOF
                        ]
            idx = child.expect(patterns, timeout=1)
            if idx == len(patterns) - 1:
                break
            responses = {0: 'group', 1: 'username', 2: 'password', 3: 'password', 4: 'notes'}
            child.sendline(params[responses[idx]])

try:
    Create()
    Populate()
except Exception as e:
    print("Exception", e)
    sys.exit(1)
