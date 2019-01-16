#! /bin/bash
# Copyright (C) 2019 Dmitry Igrishin
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.

# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:

# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

# ------------------------------------------------------------------------------
# Commonly-used variables

script_dir=$(dirname "$0")
script_name=$(basename "$0")

# ------------------------------------------------------------------------------
# Commonly-used functions

# Every script can define a function exit_cleanly() with just
# one argument - exit code. This function will be called by err().
# By default, this function just exits with specified code.
#
# $1 - The exit code.
exit_cleanly()
{
  exit $1
}

# Prints the messages on the standard error.
msg()
{
  echo -e "$script_name: $@" >&2
}

# Prints the messages on the standard error without newline.
msg_n()
{
  echo -e -n "$script_name: $@" >&2
}

# Prints the messages on the standard error and exit.
#
# $1 - The exit code.
err()
{
  local code=$1
  shift
  msg "$@"
  exit_cleanly $code
}
