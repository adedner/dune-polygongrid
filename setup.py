import os, sys
try:
    from dune.packagemetadata import metaData
except ImportError:
    from packagemetadata import metaData
from skbuild import setup
setup(**metaData('2.9.0.dev20211021')[1])
