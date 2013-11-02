#!/usr/bin/env perl

# This file is part of gpwsafe.
#
# Gpwsafe is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Gpwsafe is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>


use strict;
use warnings;
use Expect;

my $gpwsafe = "../src/gpwsafe";
my $test_file = "/tmp/test.ps3";
my $password = "asdfqwer";

my @records = (
    {
        'add_req'   => 'bar.foo',
        'name'      => 'foo',
        'group'     => 'bar',
        'username'  => 'foo@bar.baz',
        'password'  => 'Qwer!234',
        'notes'     => 'First account',

        'query_req' => 'bar.foo'
    },
    {
        'add_req'   => 'baz',
        'name'      => 'baz',
        'group'     => 'bar',
        'username'  => 'foo@bar.baz',
        'password'  => 'ZxcvAsdf',
        'notes'     => 'Second account',

        'query_req' => 'bar.baz'
    }
);

unlink $test_file;
if (-e $test_file) {
    die "Failed to remove $test_file: $!\n";
}

sub create() {
    my $exp = Expect->spawn($gpwsafe, "--use-weak-randomness-for-tests",
                            "-f", $test_file, "--create")
        or die "Can't spawn command: $!\n";

    $exp->expect(1, "Enter passphrase for $test_file: ") or die "Failed";
    $exp->send("$password\n");
    $exp->expect(1, "Reenter passphrase for $test_file: ") or die "Failed";
    $exp->send("$password\n");
    $exp->expect(1, "") or die "Failed";

    $exp->soft_close();
}

sub populate() {
    foreach my $params (@records) {

        my $exp = Expect->spawn($gpwsafe, "--use-weak-randomness-for-tests",
                                "-f", $test_file, "--add", "$params->{'add_req'}")
            or die "Can't spawn command: $!\n";
        $exp->expect(1, "Enter password for $test_file: ") or die "Failed";
        $exp->send("$password\n");

        $exp->expect(1,
                     [ qr/username: /,  sub { my $self = shift;
                                              $self->send("$params->{'username'}\n");
                                              exp_continue; }],
                     [ qr/password \[return for random\]: /,
                                        sub { my $self = shift;
                                              $self->send("$params->{'password'}\n");
                                              exp_continue; }],
                     [ qr/password again: /,
                                        sub { my $self = shift;
                                              $self->send("$params->{'password'}\n");
                                              exp_continue; }],
                     [ qr/notes: /,
                                        sub { my $self = shift;
                                              $self->send("$params->{'notes'}\n");
                                              exp_continue; }],
                     [ qr/group \[<none>\]: /,
                                        sub { my $self = shift;
                                              $self->send("$params->{'group'}\n");
                                              exp_continue; }]
        );

        $exp->expect(1, "") or die "Failed";
        $exp->soft_close();
    }
}

sub check_list() {
    my $exp = Expect->spawn($gpwsafe, "--use-weak-randomness-for-tests",
                            "-f", $test_file)
        or die "Can't spawn command: $!\n";

    $exp->expect(1, "Enter password for $test_file: ") or die "Failed";
    $exp->send("$password\n");

    $exp->expect(1);
    my $a = $exp->before();
    $a =~ s/^\s+|\s+$//; # trim

    $exp->soft_close();

    my @actual = sort split("\n", $a);
    my @expected = sort map { $_->{'group'}.".".$_->{'name'} } @records;

    $#actual == $#expected or die "Lists' lengths don't match\n";
    foreach (my $i = 0; $i < $#actual; $i++) {
        $actual[$i] =~ s/^\s+|\s+$//;
        $actual[$i] eq $expected[$i]
            or die "Lists don't match at position $i: $actual[$i] != $expected[$i]\n";
    }
}

sub check_passwords() {
    foreach my $params (@records) {

        my $exp = Expect->spawn($gpwsafe, "--use-weak-randomness-for-tests",
                                "-f", $test_file, "-Eup", "$params->{'query_req'}")
            or die "Can't spawn command: $!\n";

        $exp->expect(1, "Going to print login and password to stdout") or die "Failed";
        $exp->expect(1, "Enter password for $test_file: ") or die "Failed";
        $exp->send("$password\n");

        $exp->expect(1, "username for ".$params->{'query_req'}.": ".$params->{'username'});
        $exp->expect(1, "password for ".$params->{'query_req'}.": ".$params->{'password'});


        $exp->expect(1, "") or die "Failed";
        $exp->soft_close();
    }
}



&create();
&populate();
&check_list();
&check_passwords();

print "PASS";

# vim: set et ts=4 sw=4:
