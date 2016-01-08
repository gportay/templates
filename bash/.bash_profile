# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

PATH=$PATH:$HOME/.local/bin:$HOME/bin

# set PATH so it includes x-tools toolchains if it exists
for dir in $HOME/x-tools/*/bin; do
	[ ! -d $dir ] && continue
	PATH="$dir:$PATH"
done

export PATH

function prompt_shell_rc {
	if [ ${1:-0} -ne 0 ]; then
		echo -e "($1) "
	fi
}

if ! echo "$PS1" | grep -q prompt_shell_rc; then
	PS1="\\033[1;31m\$(prompt_shell_rc \$?)\\033[0m$PS1"
fi

function prompt_git_branch {
	git branch --no-color 2>/dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\ \[\1\]/'
}

if ! echo "$PS1" | grep -q prompt_git_branch && which git; then
	PS1="$(echo "$PS1" | sed 's/\(\\$ \)$/\\033[1;32m$(prompt_git_branch)\\033[0m\1/1')"
fi >/dev/null 2>&1
