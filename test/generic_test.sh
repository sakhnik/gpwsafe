#!/bin/bash

set -e

test_file=/tmp/test.ps3
gpwsafe="$1 --use-weak-randomness-for-tests -f $test_file"
password=asdfqwer

rm -f $test_file

echo -e "\nCreate"

expect <<END
set timeout 500
spawn $gpwsafe --create
expect -exact "Enter passphrase for $test_file: " { send "$password\r" }
expect -exact "Reenter passphrase for $test_file: " { send "$password\r" }
expect eof
wait
END

echo -e "\nPopulate 1"

expect <<END
set timeout 500
spawn $gpwsafe --add bar.foo
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact "username: " { send "foo\r" }
expect -exact {password [return for random]: } { send "Qwer!234\r" }
expect -exact "password again: " { send "Qwer!234\r" }
expect -exact "notes: " { send "First account\r" }
expect eof
wait
END

echo -e "\nPopulate 2"

expect <<END
set timeout 500
spawn $gpwsafe --add baz
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact {group [<none>]: } { send "bar\r" }
expect -exact "username: " { send "baz\r" }
expect -exact {password [return for random]: } { send "ZxcvAsdf\r" }
expect -exact "password again: " { send "ZxcvAsdf\r" }
expect -exact "notes: " { send "Second account\r" }
expect eof
wait
END

echo -e "\nCheck list"

expect <<END
set timeout 500
spawn $gpwsafe
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact "bar.baz"
expect -exact "bar.foo"
expect eof
wait
END

echo -e "\nCheck password 1"

expect <<END
set timeout 500
spawn $gpwsafe -Eup bar.foo
expect -exact "Going to print login and password to stdout"
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect "username for bar.foo: foo"
expect "password for bar.foo: Qwer!234"
expect eof
wait
END

echo -e "\nCheck password 2"

expect <<END
set timeout 500
spawn $gpwsafe -Eup bar.baz
expect -exact "Going to print login and password to stdout"
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect "username for bar.baz: baz"
expect "password for bar.baz: ZxcvAsdf"
expect eof
wait
END

echo -e "\nCheck info 1"

expect <<END
set timeout 500
spawn $gpwsafe bar.foo
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect "Group:\t\tbar"
expect "Title:\t\tfoo"
expect "User:\t\tfoo"
expect "Notes:\t\tFirst account"
expect "Pass:\t\t*************"
expect eof
wait
END

echo -e "\nCheck info 2"

expect <<END
set timeout 500
spawn $gpwsafe bar.baz
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect "Group:\t\tbar"
expect "Title:\t\tbaz"
expect "User:\t\tbaz"
expect "Notes:\t\tSecond account"
expect "Pass:\t\t*************"
expect eof
wait
END

echo -e "\nCheck delete"

expect <<END
set timeout 500
spawn $gpwsafe --delete bar.baz
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact {Confirm deleting bar.baz ? [n]} { send "y\r" }
expect eof
wait

spawn $gpwsafe --delete bar.foo
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact {Confirm deleting bar.foo ? [n]} { send "y\r" }
expect eof
wait

spawn $gpwsafe
expect -exact "Enter password for $test_file: " { send "$password\r" }
expect -exact "No matching entries found"
expect eof
wait
END

echo -e "\nPASS"
