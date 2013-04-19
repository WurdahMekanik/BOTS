# Keybinds
bindkey '^[[7~' vi-beginning-of-line
bindkey "^[[8~" vi-end-of-line
bindkey "^[Od"  vi-backward-word
bindkey "^[Oc"  vi-forward-word
bindkey "^[[A"  history-beginning-search-backward
bindkey "^[[B"  history-beginning-search-forward
bindkey "^[[3~" delete-char

# The following lines were added by compinstall
zstyle ":completion:*" completer _list _oldlist _expand _complete _ignored _match _correct _approximate _prefix
zstyle ":completion:*" completions 1
zstyle ":completion:*" format 'Completing %d'
zstyle ":completion:*" glob 1
zstyle ":completion:*" group-name ''
zstyle ":completion:*" list-colors ''
zstyle ":completion:*" list-prompt %SAt %p: Hit TAB for more, or the character to insert%s
zstyle ":completion:*" matcher-list '' '+m:{[:lower:][:upper:]}={[:upper:][:lower:]}' 'r:|[._-]=* r:|=*' 'l:|=* r:|=*'
zstyle ":completion:*" max-errors 2 numeric
zstyle ":completion:*" menu select=1
zstyle ":completion:*" prompt 'Corrections ($e)'
zstyle ":completion:*" select-prompt %SScrolling active: current selection at %p%s
zstyle ":completion:*" substitute 1
zstyle :compinstall filename '/home/quorque/.zshrc'

autoload -Uz compinit colors
compinit -u
colors
alsi -l -u 
source /usr/share/zsh/plugins/zsh-syntax-highlight/zsh-syntax-highlighting.zsh

# Aliases
alias grep="grep -n --color=auto"
alias ls="ls --color=auto"
alias ll="ls -l --color=auto"
eval $(dircolors -b)
alias vi=vim
alias sudo="sudo "
alias arduino="gksudo arduino"
alias piklab="gksudo piklab"
alias pytwo=python2.7

# End of lines added by compinstall
# Lines configured by zsh-newuser-install
HISTFILE=~/.histfile
HISTSIZE=1000
SAVEHIST=1000
setopt appendhistory autocd extendedglob notify
unsetopt beep
bindkey -e
# End of lines configured by zsh-newuser-install

# Custom Prompt settings
PROMPT="%{$fg[blue]%}%n%{$fg[white]%}@%{$fg[green]%}%m%{$reset_color%} %{$fg[red]%}[%D{%d. %b %Y}%{$fg[cyan] | %{$fg[red]%}%*] %{$fg[cyan]- %{$fg[yellow]%}{%~}""
%{$fg[cyan]%}:%#>"%{$reset_color%} 
