" Enable line numbers
set number

" Autoindentation
set autoindent

" Enables color coding of syntax
filetype on
filetype plugin on
syntax enable
set grepprg=grep\ -mH\ $*

" Enable mouse in console
set mouse=a

" Awesome space stuff
nnoremap <space> za
set foldmethod=indent

" Spaces instaed of tabd
set expandtab
set smarttab

" 4 character tab
set shiftwidth=4
set softtabstop=4
