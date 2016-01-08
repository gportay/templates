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
