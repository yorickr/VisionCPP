#!/usr/bin/env python

from ronin.cli import cli
from ronin.contexts import new_context, current_context
from ronin.gcc import GccCompile, GccLink, configure_gcc
from ronin.phases import Phase
from ronin.pkg_config import Package
from ronin.projects import Project
from ronin.utils.paths import glob

def debug_hook(executor):
    executor.standard('c++11')
    with current_context() as ctx:
        print(ctx.get('build.debug'))
        if ctx.get('build.debug') == False:
            executor.optimize('2') # make sure we use '2' instead of the default 'g', as Apple's clang doesn't support this.
        else:
            executor.enable_debug()
            executor.optimize('0')

with new_context() as ctx:
    project = Project('VisionCPP')
    extensions = [Package('OpenCV')]

    configure_gcc(gcc_command='g++',
                  ccache=False)

    compile = Phase()
    compile.executor = GccCompile()
    compile.extensions += extensions
    compile.inputs = glob('*.cpp')
    compile.executor.hooks.append(debug_hook)

    link = Phase()
    link.executor = GccLink()
    link.inputs_from.append(compile)
    link.extensions += extensions
    link.output = 'opencv'
    link.executor.hooks.append(debug_hook)
    if ctx.build.run:
        link.run_output = 1

    project.phases['link'] = link
    project.phases['compile'] = compile

    cli(project)
