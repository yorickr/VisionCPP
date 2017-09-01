#!/usr/bin/env python

from ronin.cli import cli
from ronin.contexts import new_context
from ronin.gcc import GccBuild
from ronin.phases import Phase
from ronin.pkg_config import Package
from ronin.projects import Project
from ronin.utils.paths import glob

with new_context() as ctx:
    project = Project('gcc opencv')

    Phase(project=project,
      name='build',
      executor=GccBuild(command='g++'),
      inputs=glob('main.cpp'),
      extensions=[Package('OpenCV')],
      output='opencv')
    cli(project)
