import subprocess
from tests import test_set

class bc:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def main():
    for test_file, expected_output, returncode in test_set:
        completed_test = subprocess.run(["./main", test_file], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
        success = True
        output = test_file + ": "
        failed_to_match_output = expected_output not in completed_test.stdout
        failed_to_match_retcode = returncode != completed_test.returncode
        if not (failed_to_match_output or failed_to_match_retcode):
            output += bc.OKGREEN + bc.BOLD + "SUCCESS" + bc.ENDC
        else:
            output += bc.FAIL + bc.BOLD + "FAIL" + bc.ENDC
            if failed_to_match_output:
                success = False
                output += "\nexpected output not found in output"
            if failed_to_match_retcode:
                success = False
                output += "\nreturn code expected {}, return code received {}"\
                            .format(bc.WARNING + str(returncode) + bc.ENDC,
                                    bc.WARNING + str(completed_test.returncode) + bc.ENDC)
        output += "\n\n"
        print(output)


if __name__ == "__main__":
    main()
