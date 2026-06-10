#!/usr/bin/env -S --default-signal bash

# ============================================================================
# PARSING TESTER FOR MINISHELL - Tests only the parsing functionality
# ============================================================================
# This tester focuses on validating the parser's ability to:
# - Correctly identify syntax errors
# - Properly tokenize input
# - Validate command syntax
# - Test builtin command parsing
# ============================================================================

MINISHELL_PATH=$(pwd)
EXECUTABLE=minishell
RUNDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)
DATE=$(date +%Y-%m-%d_%H.%M.%S)
OUTDIR=$MINISHELL_PATH/parsing_test_output_$DATE
TMP_OUTDIR=$(mktemp -d)
TMP_TESTDIR=$(mktemp -d)

# Colors
RESET="\033[0m"
BOLD="\033[1m"
ITALIC="\033[3m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
GRAY="\033[90m"

# Test counters
TEST_COUNT=0
TESTS_PASSED=0
TESTS_OK=0
TESTS_KO=0
SYNTAX_ERRORS_DETECTED=0
SYNTAX_ERRORS_MISSED=0

main() {
	trap sigint_trap SIGINT
	trap cleanup EXIT

	echo -e "${BOLD}${YELLOW}# **************************************************************************** #"
	echo "#                    PARSING TESTER FOR MINISHELL                      #"
	echo -e "# **************************************************************************** #${RESET}"

	if [[ ! -f $MINISHELL_PATH/$EXECUTABLE ]] ; then
		echo -e "${BOLD}${BLUE}# **************************************************************************** #"
		echo "#                            MINISHELL NOT COMPILED                            #"
		echo "#                                 COMPILING ...                                #"
		echo -e "# **************************************************************************** #${RESET}"
		if ! make -s -C $MINISHELL_PATH ; then
			echo -e "${BOLD}${RED}COMPILATION FAILED${RESET}"
			exit 1
		fi
		echo -e "${BOLD}${BLUE}# **************************************************************************** #${RESET}"
	elif ! make --question -s -C $MINISHELL_PATH &>/dev/null ; then
		echo -e "${BOLD}${BLUE}# **************************************************************************** #"
		echo "#                           MINISHELL NOT UP TO DATE                           #"
		echo "#                                 COMPILING ...                                #"
		echo -e "# **************************************************************************** #${RESET}"
		if ! make -s -C $MINISHELL_PATH ; then
			echo -e "${BOLD}${RED}COMPILATION FAILED${RESET}"
			exit 1
		fi
		echo -e "${BOLD}${BLUE}# **************************************************************************** #${RESET}"
	fi

	print_title "PARSING TESTS" "🔍"
	run_parsing_tests

	print_title "BUILTIN PARSING TESTS" "🔨"
	run_builtin_tests

	if [[ $TEST_COUNT -gt 0 ]] ; then
		print_stats
	fi

	exit 0
}

print_title() {
	local title="$1"
	local s="$2"
	local total_length=80
	local title_length=${#title}
	local padding_length_left=$(( (total_length - title_length - 4) / 2 ))
	local padding_length_right=$((padding_length_left + (total_length - title_length - 4) % 2))
	local padding_left=$(printf '%*s' "$padding_length_left" "")
	local padding_right=$(printf '%*s' "$padding_length_right" "")

	echo "  $s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s"
	echo -e "  $s${padding_left}${BOLD}${BLUE}$title${RESET}${padding_right}$s"
	echo "  $s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s$s"
}

run_parsing_tests() {
	# Test 1: Valid simple command
	test_parsing "echo hello" 0 "Simple command"

	# Test 2: Valid pipe
	test_parsing "echo hello | cat" 0 "Pipe command"

	# Test 3: Valid redirection
	test_parsing "echo hello > file.txt" 0 "Output redirection"

	# Test 4: Valid input redirection
	test_parsing "cat < file.txt" 0 "Input redirection"

	# Test 5: Valid append redirection
	test_parsing "echo hello >> file.txt" 0 "Append redirection"

	# Test 6: Valid heredoc
	test_parsing "cat << EOF" 0 "Heredoc start"

	# Test 7: Valid background command
	test_parsing "sleep 10 &" 0 "Background command"

	# Test 8: Valid and operator
	test_parsing "echo hello && echo world" 0 "AND operator"

	# Test 9: Valid or operator
	test_parsing "echo hello || echo world" 0 "OR operator"

	# Test 10: Valid command list with semicolon
	test_parsing "echo hello; echo world" 0 "Semicolon separator"

	# Test 11: Pipe at start (SYNTAX ERROR)
	test_parsing "| echo hello" 1 "Pipe at start - should fail"

	# Test 12: Pipe at end (SYNTAX ERROR)
	test_parsing "echo hello |" 1 "Pipe at end - should fail"

	# Test 13: Double pipe (SYNTAX ERROR)
	test_parsing "echo hello || |" 1 "Double pipe - should fail"

	# Test 14: Unclosed quote (SYNTAX ERROR)
	test_parsing "echo \"hello" 1 "Unclosed double quote - should fail"

	# Test 15: Unclosed single quote (SYNTAX ERROR)
	test_parsing "echo 'hello" 1 "Unclosed single quote - should fail"

	# Test 16: Empty redirector (SYNTAX ERROR)
	test_parsing "echo hello >" 1 "Empty redirector - should fail"

	# Test 17: Double redirector (SYNTAX ERROR)
	test_parsing "echo hello > > file.txt" 1 "Double redirector - should fail"

	# Test 18: Unmatched parenthesis (SYNTAX ERROR)
	test_parsing "(echo hello" 1 "Unmatched open parenthesis - should fail"

	# Test 19: Valid subshell command
	test_parsing "(echo hello)" 0 "Valid subshell"

	# Test 20: Multiple pipes
	test_parsing "cat file.txt | grep pattern | wc -l" 0 "Multiple pipes"

	# Test 21: Pipe with redirection
	test_parsing "cat file.txt | grep pattern > output.txt" 0 "Pipe with redirection"

	# Test 22: Variable expansion
	test_parsing "echo \$USER" 0 "Variable expansion"

	# Test 23: Single quote prevents expansion
	test_parsing "echo '\$USER'" 0 "Single quote variable"

	# Test 24: Command substitution
	test_parsing "echo \$(whoami)" 0 "Command substitution"

	# Test 25: Backtick substitution
	test_parsing "echo \`whoami\`" 0 "Backtick substitution"

	# Test 26: Consecutive operators
	test_parsing "echo a && echo b || echo c" 0 "Consecutive operators"

	# Test 27: Whitespace handling
	test_parsing "   echo   hello   " 0 "Extra whitespace"

	# Test 28: Tab characters
	test_parsing $'echo\t\thello' 0 "Tab characters"

	# Test 29: Empty command (SYNTAX ERROR)
	test_parsing "" 1 "Empty command - should fail"

	# Test 30: Only whitespace
	test_parsing "    " 1 "Only whitespace - should fail"

	# Test 31: Only pipes
	test_parsing "|" 1 "Only pipe - should fail"

	# Test 32: Redirect to pipe (SYNTAX ERROR)
	test_parsing "echo hello > |" 1 "Redirect to pipe - should fail"

	# Test 33: Complex valid command
	test_parsing "grep -r \"pattern\" . | sort | uniq -c > results.txt" 0 "Complex valid command"

	# Test 34: Command with options
	test_parsing "ls -la -h /tmp" 0 "Command with multiple options"

	# Test 35: Quote in middle
	test_parsing "echo hello\"world" 0 "Quote in middle"

	# Test 36: Escaped characters
	test_parsing "echo \\\"hello\\\"" 0 "Escaped quotes"

	# Test 37: Mixed quotes
	test_parsing "echo 'hello' \"world\"" 0 "Mixed quotes"

	# Test 38: Semicolon in middle
	test_parsing "echo a ; echo b ; echo c" 0 "Multiple semicolons"

	# Test 39: Ampersand without space
	test_parsing "echo hello&" 0 "Ampersand without space"

	# Test 40: Pipe without space
	test_parsing "echo|cat" 0 "Pipe without space"
}

run_builtin_tests() {
	# ECHO Tests
	test_parsing "echo" 0 "Echo no args"
	test_parsing "echo hello" 0 "Echo with args"
	test_parsing "echo -n hello" 0 "Echo with -n flag"
	test_parsing "echo -ne hello" 0 "Echo with -ne flags"
	test_parsing "echo hello world" 0 "Echo multiple args"

	# CD Tests
	test_parsing "cd" 0 "CD no args"
	test_parsing "cd /tmp" 0 "CD with path"
	test_parsing "cd -" 0 "CD with dash"
	test_parsing "cd ~" 0 "CD with tilde"
	test_parsing "cd .." 0 "CD parent directory"

	# PWD Tests
	test_parsing "pwd" 0 "PWD builtin"

	# EXPORT Tests
	test_parsing "export VAR=value" 0 "Export with assignment"
	test_parsing "export VAR" 0 "Export without assignment"
	test_parsing "export VAR1=val1 VAR2=val2" 0 "Export multiple vars"
	test_parsing "export" 0 "Export with no args"

	# UNSET Tests
	test_parsing "unset VAR" 0 "Unset single variable"
	test_parsing "unset VAR1 VAR2" 0 "Unset multiple variables"
	test_parsing "unset" 0 "Unset with no args"

	# ENV Tests
	test_parsing "env" 0 "Env builtin"

	# EXIT Tests
	test_parsing "exit" 0 "Exit with no args"
	test_parsing "exit 0" 0 "Exit with code 0"
	test_parsing "exit 42" 0 "Exit with code 42"
	test_parsing "exit 256" 0 "Exit with large code"

	# BUILTIN with PIPES and REDIRECTIONS
	test_parsing "echo hello | cat" 0 "Echo piped to cat"
	test_parsing "echo test > file.txt" 0 "Echo redirected to file"
	test_parsing "export VAR=val && echo \$VAR" 0 "Export with AND operator"
	test_parsing "cd /tmp && pwd" 0 "CD and PWD with AND"

	# BUILTIN ERROR CASES
	test_parsing "echo >" 1 "Echo with empty redirect - should fail"
	test_parsing "export" 0 "Export with no args (valid)"
	test_parsing "cd /nonexistent/path/that/does/not/exist" 0 "CD to nonexistent (parsing valid)"

	# Multiple Builtins
	test_parsing "pwd && cd /tmp && pwd" 0 "Multiple builtins with AND"
	test_parsing "export VAR=1; echo \$VAR" 0 "Multiple builtins with semicolon"
	test_parsing "export VAR=1 || echo error" 0 "Export with OR operator"

	# Builtin with Complex Arguments
	test_parsing "echo 'hello world'" 0 "Echo with quoted string"
	test_parsing "export \"VAR=value with spaces\"" 0 "Export with quoted value"
	test_parsing "cd '/tmp/some path'" 0 "CD with quoted path"

	# Builtin with Command Substitution
	test_parsing "export VAR=\$(echo value)" 0 "Export with command substitution"
	test_parsing "cd \$(pwd)" 0 "CD with command substitution"
	test_parsing "echo \$(env | grep PATH)" 0 "Echo with piped substitution"

	# Builtin with Variable Expansion
	test_parsing "cd \$HOME" 0 "CD with variable"
	test_parsing "export VAR=\$HOME/path" 0 "Export with variable expansion"
	test_parsing "echo \$VAR" 0 "Echo variable"
	test_parsing "echo \$VAR:\$PATH" 0 "Echo multiple variables"

	# Invalid Builtin Syntax
	test_parsing "export =value" 1 "Export with no variable name - should fail"
	test_parsing "cd /path1 /path2" 0 "CD with multiple args (parsing ok, runtime may fail)"
	test_parsing "exit abc" 0 "Exit with non-numeric (parsing ok, runtime may fail)"
}

test_parsing() {
	local input="$1"
	local expected_exit_code="$2"
	local description="$3"
	local exit_minishell
	local exit_bash
	local status="❌"
	local result="FAIL"
	local display_input

	((TEST_COUNT++))

	# Run the test with minishell (send EOF immediately to test parsing only)
	cd "$TMP_TESTDIR" &>/dev/null
	echo -n "$input" | $MINISHELL_PATH/$EXECUTABLE >/dev/null 2>&1
	exit_minishell=$?
	
	# Run the same with bash for reference
	echo -n "$input" | bash >/dev/null 2>&1
	exit_bash=$?
	cd - &>/dev/null

	# Determine if parsing was successful
	# Exit code 2 typically indicates syntax error in bash
	local minishell_syntax_error=0
	local bash_syntax_error=0

	if [[ $exit_minishell -eq 2 ]] || [[ $exit_minishell -eq 1 && $exit_minishell -ne 0 ]]; then
		minishell_syntax_error=1
	fi

	if [[ $exit_bash -eq 2 ]] || [[ $exit_bash -eq 1 ]]; then
		bash_syntax_error=1
	fi

	# Compare results
	if [[ $expected_exit_code -eq 1 ]]; then
		# Should be a syntax error
		if [[ $minishell_syntax_error -eq 1 ]]; then
			status="✅"
			result="PASS"
			((TESTS_OK++))
			((TESTS_PASSED++))
			((SYNTAX_ERRORS_DETECTED++))
		else
			status="❌"
			result="FAIL"
			((TESTS_KO++))
			((SYNTAX_ERRORS_MISSED++))
		fi
	else
		# Should parse successfully
		if [[ $minishell_syntax_error -eq 0 ]]; then
			status="✅"
			result="PASS"
			((TESTS_OK++))
			((TESTS_PASSED++))
		else
			status="❌"
			result="FAIL"
			((TESTS_KO++))
		fi
	fi

	# Truncate input for display
	if [[ ${#input} -gt 30 ]]; then
		display_input="${input:0:27}..."
	else
		display_input="$input"
	fi

	printf "${BOLD}${MAGENTA}%-3s${RESET}" "$TEST_COUNT:"
	printf " %s " "$status"
	printf "${BOLD}${CYAN}%-50s${RESET}" "$description"
	printf " [${GRAY}Input: %-30s${RESET}]\n" "\"$display_input\""

	if [[ "$result" != "PASS" ]]; then
		printf "     ${BOLD}${RED}Expected: exit_code=$expected_exit_code, Got: minishell=$exit_minishell, bash=$exit_bash${RESET}\n"
	fi
}

print_stats() {
	local total_passed=$TESTS_OK
	local total_failed=$TESTS_KO

	echo ""
	echo "🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁"
	echo -e "🏁                                  ${BOLD}${RED}PARSING RESULTS${RESET}                              🏁"
	echo "🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁"
	echo ""
	print_centered "${BOLD}${MAGENTA}TOTAL TESTS: $TEST_COUNT${RESET}  ${BOLD}${GREEN}PASSED: $TESTS_PASSED${RESET}  ${BOLD}${RED}FAILED: $TESTS_KO${RESET}"
	echo ""
	print_centered "${BOLD}${CYAN}Syntax Errors Detected: ${BOLD}${GREEN}$SYNTAX_ERRORS_DETECTED${RESET}  ${BOLD}${CYAN}Syntax Errors Missed: ${BOLD}${RED}$SYNTAX_ERRORS_MISSED${RESET}"
	echo ""
	echo "🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁🏁"

	if [[ $TESTS_KO -eq 0 ]]; then
		echo -e "${BOLD}${GREEN}✅ ALL PARSING TESTS PASSED! ✅${RESET}"
	else
		echo -e "${BOLD}${RED}❌ $TESTS_KO TEST(S) FAILED ❌${RESET}"
	fi
}

strip_ansi() {
	echo -ne "${1}" | sed -E "s/(\033|\x1B|\x1b|\e)\[(([0-9]{1,3};)*[0-9]{1,3})?[mGK]//g"
}

print_centered() {
	local text=$1
	local total_length=82
	local pure_text="$(strip_ansi "$text")"
	local text_length=${#pure_text}
	local padding=$(( (total_length - text_length + 1) / 2 ))

	printf "%*s%b\n" $padding "" "$text"
}

cleanup() {
	rm -rf "$TMP_OUTDIR" 2>/dev/null
	rm -rf "$TMP_TESTDIR" 2>/dev/null
}

sigint_trap() {
	cleanup
	exit 130
}

# Start the tester
main "$@"