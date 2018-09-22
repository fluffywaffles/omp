# Make Variables and Phases and Oh My
# ========================================================================
# Because, hey, let's make this as crufty and confusing as possible!
#
# Make "expands" variables. And if that sounds like macros to you, go
# home! You're drunk. This isn't Scheme; it's MAKE!
#
# Make expands in 2 phases. Unless you add a .SECONDEXPANSION, in which
# case fuck you.
#
# Make variables have only about a half-dozen different (?) assignment
# operators, some of which are even idempotent... Unless environment
# variables are specified and the `-e` flag is used. Or, you know, maybe
# something else happens, like you define the same variable twice. That's
# not an error! Errors are bad. Make good; error bad. ... Hooray.
#
# You've got =, ?=, :=, ::=, +=, !=, and even a fancy-shmancy `define`
# keyword that... well, let's just explain them all first:
#
# =   : late-binding, so that at runtime you can have fun unraveling it.
# ?=  : =, but only if the variable doesn't already exist.
# :=  : =, but immediately-bound, so that at runtime you're stuck with it.
# +=  : the "upend" operator: append if exists, otherwise initialize.
# ::= : this is just the same as :=, because fuck you again!
# !=  : shell assignment: hey, finally, something useful!
#
# And then any of those can *also* be written as:
#
# define <variable-name> [<assignment-op>]
#   <assignment-body>
# endef
#
# where omitting <assignment-op> is the same as =. Why does define exist?
# This isn't Scheme; it's MAKE! Then again, I mean, where better to
# express your individuality than in a supposedly platform-independent,
# deterministic, hopefully-even-idempotent build script for your software
# so that it's even less likely anyone will be able to understand your
# build! Sure! That makes great sense. Let's just have a million ways to
# do everything. A+. Purple Heart. Game over. Thanks, Stallman‒I mean,
# Feldman‒I mean, Obama.
#
# ========================================================================

# NOTE: .PHONY means: the rule is not associated with an output file.
# PHONYs be trippin' all up in this game. They don't MAKE output, man.
.PHONY: clean .create-target-directory

# Which one does what it looks like? None of them! All of them! Make!
# NOTE: $$? is necessary to escape the `$` in the shell variable `$?`.
target-directory-exists != test -d $(target-directory); echo $$?
# target-directory-exists := $$(test -d $(target-directory); echo $$?)
# target-directory-exists := $(shell test -d $(target-directory); echo $$?)
# target-directory-exists ::= $(shell test -d $(target-directory); echo $$?)

# NOTE: Adding an extra `$` also *escapes* obligation for monetary reward.
# $$20 to the first person who figures out why this doesn't work!
# .target-directory-dir:
# 	ifneq ($(target-directory-exists), 0)
# 		mkdir -p bin
# 	endif

# BECAUSE OBVIOUSLY IF YOU INDENT SOMETHING IT'S NOT MAKE ANYMORE IT'S SH
.create-target-directory:
ifneq ($(target-directory-exists), 0)
	mkdir -p $(target-directory)
endif

# Some actual configuration: flags and such
cc      = clang
ccflags = -fopenmp -o bin/$@
compile = ${cc} ${ccflags}

target-directory = bin

# NOTE: run these rules before any compilation
pre-rule-dependencies = .create-target-directory

# By default, make everything
all: hello-world pi

# <target>: <dependencies>[; <recipe-0>] (\n\t<recipe-n>)...
# The "target", accesible as $@, is the expected output
# The "dependencies" are the files or other rules which must exist/be run
# The "recipe" lines are individual commands to be run in a subshell
# $< is the first prerequisite (in our case, conventionally the source)
hello-world: hello.c $(pre-rule-dependencies)
	${compile} $<

pi: pi.c $(pre-rule-dependencies)
	${compile} $<

clean:
	rm -rf bin
