import subprocess
from tests import test_set

class bc:
    _HEADER = '\033[95m'
    _OKBLUE = '\033[94m'
    _OKGREEN = '\033[92m'
    _WARNING = '\033[93m'
    _FAIL = '\033[91m'
    _ENDC = '\033[0m'
    _BOLD = '\033[1m'
    _UNDERLINE = '\033[4m'

    @staticmethod
    def HEADER(s):
        return bc._HEADER + str(s) + bc._ENDC

    @staticmethod
    def OKBLUE(s):
        return bc._OKBLUE + str(s) + bc._ENDC

    @staticmethod
    def OKGREEN(s):
        return bc._OKGREEN + str(s) + bc._ENDC

    @staticmethod
    def WARNING(s):
        return bc._WARNING + str(s) + bc._ENDC

    @staticmethod
    def FAIL(s):
        return bc._FAIL + str(s) + bc._ENDC

    @staticmethod
    def BOLD(s):
        return bc._BOLD + str(s) + bc._ENDC

    @staticmethod
    def UNDERLINE(s):
        return bc._UNDERLINE + str(s) + bc._ENDC

def main():
    for test_file, expected_output, returncode in test_set:
        completed_test = subprocess.run(["./main", test_file], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
        test_out = completed_test.stdout

        success = True
        output = bc.HEADER(test_file) + ": "
        failed_to_match_output = expected_output not in test_out
        failed_to_match_retcode = returncode != completed_test.returncode
        if not (failed_to_match_output or failed_to_match_retcode):
            output += bc.OKGREEN(bc.BOLD("SUCCESS"))
        else:
            output += bc.FAIL(bc.BOLD("FAIL"))
            if failed_to_match_output:
                success = False
                output += "\nexpected output {}\nnot found in output {}"\
                            .format(bc.WARNING(expected_output),
                                    bc.WARNING(test_out))
            if failed_to_match_retcode:
                success = False
                output += "\nreturn code expected {}, return code received {}"\
                            .format(bc.WARNING(returncode),
                                    bc.WARNING(completed_test.returncode))
        output += "\n"
        print(output)


if __name__ == "__main__":
    main()
