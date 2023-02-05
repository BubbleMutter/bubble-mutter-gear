CONDA_PREFIX="/cygdrive/d/app-devel/Anaconda3"

# anaconda
NPATH=$NPATH:$CONDA_PREFIX
NPATH=$NPATH:$CONDA_PREFIX/Library/mingw-w64/bin
NPATH=$NPATH:$CONDA_PREFIX/Library/usr/bin
NPATH=$NPATH:$CONDA_PREFIX/Library/bin
NPATH=$NPATH:$CONDA_PREFIX/Scripts
PATH=$NPATH:$PATH

export PATH
export PYTHONPATH=/usr/lib/python3.6/site-packages/
