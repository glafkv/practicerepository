# .bashrc


# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

set autoindent

# User specific aliases and functions

TERM=xterm-color

alias k='clear'
alias rm='rm -i'
export PATH=$PATH:.
