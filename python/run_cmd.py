import subprocess
import os
def run_and_print(args):
    cmd=''
    for word in args:
        cmd=cmd+' '+word
    print('<<< '+cmd+':')
    result= subprocess.run(args,stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    print(result.stdout)
    print(result.stderr)
    print('return status '+str(result.returncode)+' >>>')
    return result.returncode


current_dir=os.getcwd()
current_source_dir = os.path.dirname(os.path.realpath(__file__)) # current sou
print(current_dir)
print(current_source_dir)

run_and_print(['git','status'])