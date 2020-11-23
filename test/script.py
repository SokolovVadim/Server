# import executing_code
import sys

def execfile(filename, globals=None, locals=None):
    if globals is None:
        globals = sys._getframe(1).f_globals
    if locals is None:
        locals = sys._getframe(1).f_locals
    with open(filename, "r") as fh:
        exec(fh.read()+"\n", globals, locals)

print('Hello from script!')
# this may cause passing arguments issues
exec(open("executing_code.py").read())
