# Use LuaLaTeX
$pdf_mode = 4;
$lualatex = 'lualatex -interaction=nonstopmode -shell-escape -recorder %O %S';

# Build into build directory
$out_dir = 'build';
$aux_dir = 'build';

# Clean Beamer auxiliary files
$clean_ext = 'nav snm vrb synctex.gz';

# Ensure build directory exists
ensure_path('TEXINPUTS', './build//');

# Force multiple passes when needed
$max_repeat = 5;

# Track .nav and .snm files (Beamer navigation/short navigation)
push @generated_exts, 'nav', 'snm', 'vrb';

# Custom dependency for Beamer - rerun when nav/snm change
add_cus_dep('nav', 'pdf', 0, 'beamer_nav');
sub beamer_nav { return 0; }

# Force recorder mode for better dependency detection
$recorder = 1;