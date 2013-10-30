#!/usr/bin/env perl

use strict;
use warnings;
use Expect;

my $gpwsafe = "./src/gpwsafe";
my $test_file = "/tmp/test.ps3";
my $password = "asdfqwer";

my @records = (
    {
        'add_req'   => 'bar.foo',
        'name'      => 'foo',
        'group'     => 'bar',
        'username'  => 'foo@bar.baz',
        'password'  => 'Qwer!234',
        'notes'     => 'First account'
    },
    {
        'add_req'   => 'foo2',
        'name'      => 'foo2',
        'group'     => 'bar',
        'username'  => 'foo@bar.baz',
        'password'  => 'ZxcvAsdf',
        'notes'     => 'Second account'
    }
);

unlink $test_file or die "Failed to remove $test_file: $!\n";

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
    my @expected = map { $_->{'group'}.".".$_->{'name'} } @records;

    $#actual == $#expected or die "Lists' lengths don't match\n";
    foreach (my $i = 0; $i < $#actual; $i++) {
        $actual[$i] =~ s/^\s+|\s+$//;
        $actual[$i] eq $expected[$i] or die "Lists don't match at position $i\n";
    }
}

&create();
&populate();
&check_list();

print "PASS";

# vim: set et ts=4 sw=4:
