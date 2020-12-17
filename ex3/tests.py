import os
import subprocess
tests = ['50', '52', '53', '54', '55', 'etc']

def getFiles(test):
    files = []
    solutions = []

    for file in os.listdir(test):
        if not file.endswith(".txt"):
            continue

        if ("_sol" in file):
            solutions.append(os.path.join("/", test, file))
        
        else:
            files.append(os.path.join("/", test, file))

    solutions.sort()
    files.sort()
    return [files, solutions]

for test in tests:
    files, solutions = getFiles(test)
    counter = len(files)
    n_tests = counter
    print('\n====',test,'====')

    for file, solution in zip(files, solutions):
        print('\nTest:', file)
        
        bashCommand = './a.out < ' + file[1:]
        normal = subprocess.run(bashCommand, stdout=subprocess.PIPE, shell = True, check = True,  text = True)
        
        raw_input = normal.stdout.split('\n')[:-1]
        parsed_input = ''.join(c for c in normal.stdout if c.isprintable() or c == '\n')
        f_output = parsed_input.split('\n')
        s_output = open(solution[1:], newline='\n').readlines()

        errors = False
        for s_line, f_line in zip(s_output, f_output):
            line = s_line.replace('\n','')
            if (line != f_line):
                print("Error:\nExpected: ",line,"\nReceived: ",f_line)
                errors = True
        
        if not errors:
            print("Passed")
        else:
            counter -= 1
    
    print('\n--- Score: ',counter,'/', n_tests)
    