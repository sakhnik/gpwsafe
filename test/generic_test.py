#!/usr/bin/env python

import pexpect, sys, os

gpwsafe = sys.argv[1]
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
            patterns = [{'pattern': '.*group \[<none>\]: ',
                         'field': 'group'},
                        {'pattern': '.*username: ',
                         'field': 'username'},
                        {'pattern': '.*password \[return for random\]: ',
                         'field': 'password'},
                        {'pattern': '.*password again: ',
                         'field': 'password'},
                        {'pattern': '.*notes: ',
                         'field': 'notes'},
                        {'pattern': '.*bytes available\. Waiting for more.*Done',
                         'action': 'ignore'},
                        {'pattern': pexpect.EOF,
                         'action': 'finish'}]
            idx = child.expect([i['pattern'] for i in patterns], timeout=1)
            try:
                action = patterns[idx]['action']
                if action == 'finish':
                    break
                if action == 'ignore':
                    continue
            except KeyError:
                pass
            child.sendline(params[patterns[idx]['field']])

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
        output.extend(line.split())
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

def CheckInfo():
    for params in records:
        child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file
                              + ' ' + params['query_req'])
        child.setecho(False)
        child.expect("\s*Enter password for " + test_file + ": ")
        child.sendline(password)
        child.expect('\s*Group:\s+' + params['group'])
        child.expect('\s*Title:\s+' + params['name'])
        child.expect('\s*User:\s+' + params['username'])
        child.expect('\s*Notes:\s+' + params['notes'])
        child.expect('\s*Pass:\s+\*{13}\s*')
        child.expect(pexpect.EOF)

def CheckDelete():
    for params in records:
        child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file
                              + ' --delete ' + params['query_req'])
        child.setecho(False)
        child.expect("\s*Enter password for " + test_file + ": ")
        child.sendline(password)
        child.expect("\s*Confirm deleting " + params['query_req'] + " \? \[n\] ", timeout=1)
        child.sendline("y");
        child.expect(pexpect.EOF, timeout=1)
    child = pexpect.spawn(gpwsafe + ' --use-weak-randomness-for-tests -f ' + test_file)
    child.setecho(False)
    child.expect("\s*Enter password for " + test_file + ": ")
    child.sendline(password)
    child.expect("\s*No matching entries found", timeout=1)
    child.expect(pexpect.EOF, timeout=1)

try:
    Create()
    Populate()
    CheckList()
    CheckPasswords()
    CheckInfo()
    CheckDelete()
    print("PASS")
except Exception as e:
    print("Exception", e)
    sys.exit(1)
