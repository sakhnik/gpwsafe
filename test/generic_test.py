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

def CheckList():
    child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file)
    child.setecho(False)
    child.expect("Enter password for " + test_file + ": ")
    child.sendline(password)
    output = []
    while True:
        line = child.readline()
        if not line:
            break
        line = line.strip()
        if not line:
            continue
        output.append(line)
    actual = sorted(output)
    expected = sorted([(i['group'] + '.' + i['name']).encode('utf-8') for i in records])
    if actual != expected:
        sys.stderr.write("List failed: %s != %s\n" % (str(actual), str(expected)))
        sys.exit(1)

def CheckPasswords():
    for params in records:
        child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file
                              + ' -Eup ' + params['query_req'])
        child.setecho(False)
        child.expect("\s*Going to print login and password to stdout", 1)
        child.expect("\s*Enter password for " + test_file + ": ")
        child.sendline(password)
        child.expect("\s*username for " + params['query_req'] + ": " + params['username'])
        child.expect("\s*password for " + params['query_req'] + ": " + params['password'])

        child.expect("\s*")
        child.expect(pexpect.EOF)

try:
    Create()
    Populate()
    CheckList()
    CheckPasswords()
    print("PASS")
except Exception as e:
    print("Exception", e)
    sys.exit(1)
