# Use LuaLaTeX
$pdf_mode = 4;
$lualatex = 'lualatex -interaction=nonstopmode -shell-escape %O %S';

# Build into build directory
$out_dir = 'build';
$aux_dir = 'build';

# Clean Beamer auxiliary files
$clean_ext = 'nav snm vrb synctex.gz';

# Ensure build directory exists
ensure_path('TEXINPUTS', './build//');