#!/usr/bin/env python2

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

def generate_project(project_path, project_name):
    project = Project(project_name)
    extensions = [Package('opencv')]

    configure_gcc(gcc_command='g++',
                  ccache=False)

    compile = Phase()
    compile.executor = GccCompile()
    compile.extensions += extensions
    compile.inputs = glob(project_path + '*.cpp')
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
    return project

with new_context() as ctx:
    project_version = int(ctx.get('project.version'))
    print("Running project version " + str(project_version))
    if project_version is 1:
        p1 = generate_project('./inleiding-vision-cpp/','VisionCPP-1')
        cli(p1)
    elif project_version is 2:
        p2 = generate_project('./vision-cpp-gevorderden/','VisionCPP-2')
        cli(p2)
    else:
        print("No project version specified")
