#!/usr/bin/env bash

# ============================================================================
# MANDATORY MINISHELL PARSING + BUILTIN TESTER
# Tailored for 42 Minishell mandatory part.
#
# What this tester checks:
#   - Mandatory-valid parsing cases: words, quotes, pipes, redirects, heredoc
#   - Mandatory syntax errors: bad pipe/redirection/quotes
#   - Unsupported mandatory syntax: ; & && || ( ) $() backticks
#   - Builtin parsing/runtime error cases for export/cd/exit
#
# Notes:
#   - This tester does NOT treat bonus syntax as valid.
#   - For syntax errors it accepts either:
#	   1) exit code 1 or 2, OR
#	   2) output containing "Syntax error" / "syntax error"
#	 This is useful while your parser is still being developed and main.c may
#	 still return 0 at EOF.
#   - Later, when execution is complete, you should make syntax errors set
#	 exit status 2 and return it in non-interactive mode.
# ============================================================================

EXECUTABLE="./minishell"
TIMEOUT_SEC=3
STRICT_EXIT=0

if [[ "$1" == "--strict-exit" ]]; then
	STRICT_EXIT=1
fi

RESET="\033[0m"
BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
CYAN="\033[36m"
GRAY="\033[90m"

TOTAL=0
PASS=0
FAIL=0
TMP_DIR=""

cleanup() {
	[[ -n "$TMP_DIR" && -d "$TMP_DIR" ]] && rm -rf "$TMP_DIR"
}
trap cleanup EXIT

print_header() {
	echo -e "${BOLD}${BLUE}============================================================${RESET}"
	echo -e "${BOLD}${BLUE} Mandatory Minishell Parsing + Builtin Tester${RESET}"
	echo -e "${BOLD}${BLUE}============================================================${RESET}"
	echo ""
}

compile_if_needed() {
	if [[ ! -f "$EXECUTABLE" ]]; then
		echo -e "${YELLOW}minishell not found. Running make...${RESET}"
		make || exit 1
	elif ! make --question &>/dev/null; then
		echo -e "${YELLOW}minishell not up to date. Running make...${RESET}"
		make || exit 1
	fi
}

run_minishell() {
	local input="$1"
	local outfile="$2"

	# printf keeps multi-line tests working, e.g. heredoc.
	# timeout prevents hanging on heredoc/unfinished readline cases.
	# The outer subshell redirects shell pipeline diagnostics too, for example
	# "Segmentation fault (core dumped)" messages from a crashing minishell.
	(printf "%b\n" "$input" | timeout "$TIMEOUT_SEC" "$EXECUTABLE" >"$outfile" 2>&1) 2>>"$outfile"
	return $?
}

contains_syntax_error() {
	local outfile="$1"
	grep -Eqi "syntax error|Syntax error|unexpected token" "$outfile"
}

contains_any_error() {
	local outfile="$1"
	grep -Eqi "syntax error|not an identifier|not a valid identifier|too many arguments|numeric argument required|No such file|cd:" "$outfile"
}

mark_result() {
	local ok="$1"
	local desc="$2"
	local input="$3"
	local code="$4"
	local outfile="$5"
	local reason="$6"

	TOTAL=$((TOTAL + 1))
	if [[ "$ok" -eq 1 ]]; then
		PASS=$((PASS + 1))
		printf "${BOLD}${CYAN}%03d${RESET} ✅ %-55s ${GRAY}[exit=%s]${RESET}\n" "$TOTAL" "$desc" "$code"
	else
		FAIL=$((FAIL + 1))
		printf "${BOLD}${CYAN}%03d${RESET} ❌ %-55s ${GRAY}[exit=%s]${RESET}\n" "$TOTAL" "$desc" "$code"
		echo -e "	 ${RED}Reason:${RESET} $reason"
		echo -e "	 ${GRAY}Input:${RESET} $input"
		echo -e "	 ${GRAY}Output:${RESET}"
		sed 's/^/	   | /' "$outfile" | head -40
	fi
}

# kind values:
#   VALID		=> should parse without syntax error
#   SYNTAX	   => should be syntax error / unsupported syntax
#   BUILTIN_ERR  => parsing is valid, but builtin should report runtime error
run_test() {
	local desc="$1"
	local input="$2"
	local kind="$3"
	local expected_pattern="$4"
	local outfile="$TMP_DIR/out_$TOTAL.txt"
	local code
	local ok=0
	local reason=""

	run_minishell "$input" "$outfile"
	code=$?

	if [[ "$code" -eq 124 ]]; then
		mark_result 0 "$desc" "$input" "$code" "$outfile" "Command timed out. Possible heredoc/readline/syntax handling hang."
		return
	fi

	if [[ "$code" -eq 139 ]] || grep -Eqi "segmentation fault|core dumped" "$outfile"; then
		mark_result 0 "$desc" "$input" "$code" "$outfile" "Minishell crashed with segmentation fault. Fix this before judging parser correctness."
		return
	fi

	case "$kind" in
		VALID)
			if contains_syntax_error "$outfile"; then
				ok=0
				reason="Expected valid mandatory input, but syntax error was printed."
			else
				ok=1
			fi
			;;
		SYNTAX)
			if [[ "$STRICT_EXIT" -eq 1 ]]; then
				if [[ "$code" -eq 2 ]]; then
					ok=1
				else
					ok=0
					reason="Strict mode: expected exit code 2 for syntax error."
				fi
			else
				if [[ "$code" -eq 1 || "$code" -eq 2 ]] || contains_syntax_error "$outfile"; then
					ok=1
				else
					ok=0
					reason="Expected syntax error message or exit code 1/2."
				fi
			fi
			;;
		BUILTIN_ERR)
			if [[ -n "$expected_pattern" ]]; then
				if grep -Eqi "$expected_pattern" "$outfile"; then
					ok=1
				else
					ok=0
					reason="Expected builtin error pattern: $expected_pattern"
				fi
			else
				if contains_any_error "$outfile"; then
					ok=1
				else
					ok=0
					reason="Expected a builtin/runtime error message."
				fi
			fi
			;;
		*)
			ok=0
			reason="Unknown test kind: $kind"
			;;
	esac

	mark_result "$ok" "$desc" "$input" "$code" "$outfile" "$reason"
}

run_valid_parsing_tests() {
	echo -e "\n${BOLD}${YELLOW}VALID MANDATORY PARSING${RESET}"

	run_test "simple echo"						 "echo hello"						 VALID
	run_test "echo without args"				   "echo"							   VALID
	run_test "echo -n"							 "echo -n hello"					  VALID
	run_test "multiple words"					  "echo hello world 42"				VALID
	run_test "extra spaces"						"   echo	hello	"				VALID
	run_test "tab separators"					  $'echo\t\thello'					 VALID
	run_test "single quotes"					   "echo 'hello world'"				 VALID
	run_test "double quotes"					   "echo \"hello world\""			 VALID
	run_test "mixed quotes"						"echo 'hello' \"world\""		   VALID
	run_test "quote in middle of word"			 "echo hello\"world\""			  VALID
	run_test "variable expansion token"			"echo \$USER"						VALID
	run_test "single quote prevents expansion"	 "echo '\$USER'"					  VALID
	run_test "double quote allows expansion"	   "echo \"\$USER\""				  VALID
	run_test "exit status expansion token"		 "echo \$?"						   VALID
	run_test "pipe with spaces"					"echo hello | cat"				   VALID
	run_test "pipe without spaces"				 "echo hello|cat"					 VALID
	run_test "multiple pipes"					  "echo hello | cat | wc -c"		   VALID
	run_test "output redirect"					 "echo hello > out.txt"			   VALID
	run_test "output redirect no spaces"		   "echo hello>out.txt"				 VALID
	run_test "append redirect"					 "echo hello >> out.txt"			  VALID
	run_test "input redirect"					  "cat < input.txt"					VALID
	run_test "input and output redirect"		   "cat < input.txt > output.txt"	   VALID
	run_test "redirect before command"			 "< input.txt cat"					VALID
	run_test "pipe with redirect"				  "cat input.txt | grep hi > out.txt"  VALID
	run_test "heredoc with delimiter only"		 "cat << EOF"						 VALID
	run_test "heredoc full body"				   $'cat << EOF\nhello\nEOF'			VALID
}

run_syntax_error_tests() {
	echo -e "\n${BOLD}${YELLOW}MANDATORY SYNTAX ERRORS${RESET}"

	run_test "pipe at start"					   "| echo hello"					   SYNTAX
	run_test "pipe at end"						 "echo hello |"					   SYNTAX
	run_test "only pipe"						   "|"								  SYNTAX
	run_test "double pipe as unsupported OR"	   "echo hello || echo bye"			 SYNTAX
	run_test "two pipes separated by space"		"echo hello | | cat"				 SYNTAX
	run_test "empty output redirect"			   "echo hello >"					   SYNTAX
	run_test "empty append redirect"			   "echo hello >>"					  SYNTAX
	run_test "empty input redirect"				"cat <"							  SYNTAX
	run_test "empty heredoc redirect"			  "cat <<"							 SYNTAX
	run_test "redirect followed by redirect"	   "echo hello > > file.txt"			SYNTAX
	run_test "redirect followed by pipe"		   "echo hello > | cat"				 SYNTAX
	# Bash accepts this: second pipeline segment has only a redirection.
	# It should create/truncate file.txt and usually print no terminal output.
	run_test "pipe followed by redirect only"	  "echo hello | > file.txt"			VALID
	run_test "unclosed double quote"			   "echo \"hello"					  SYNTAX
	run_test "unclosed single quote"			   "echo 'hello"						SYNTAX
}

run_unsupported_mandatory_tests() {
	echo -e "\n${BOLD}${YELLOW}UNSUPPORTED IN MANDATORY => SHOULD FAIL${RESET}"

	run_test "semicolon separator unsupported"	 "echo hello; echo world"			 SYNTAX
	run_test "semicolon with spaces unsupported"   "echo hello ; echo world"			SYNTAX
	run_test "only semicolon unsupported"		  ";"								  SYNTAX
	run_test "ampersand unsupported"			   "sleep 1 &"						  SYNTAX
	run_test "ampersand no space unsupported"	  "echo hello&"						SYNTAX
	run_test "AND operator unsupported"			"echo a && echo b"				   SYNTAX
	run_test "OR operator unsupported"			 "echo a || echo b"				   SYNTAX
	run_test "open parenthesis unsupported"		"(echo hello"						SYNTAX
	run_test "subshell unsupported"				"(echo hello)"					   SYNTAX
	run_test "close parenthesis unsupported"	   "echo hello)"						SYNTAX
	run_test "command substitution unsupported"	"echo \$(whoami)"					SYNTAX
	run_test "backtick substitution unsupported"   'echo `whoami`'					  SYNTAX
}

run_builtin_tests() {
	echo -e "\n${BOLD}${YELLOW}BUILTIN PARSING / BUILTIN ERRORS${RESET}"

	# Valid builtin parsing cases
	run_test "pwd builtin"						 "pwd"								VALID
	run_test "env builtin"						 "env"								VALID
	run_test "export no args"					  "export"							 VALID
	run_test "export valid assignment"			 "export VAR=value"				   VALID
	run_test "export valid multiple assignment"	"export A=1 B=2"					 VALID
	run_test "export valid identifier no value"	"export MY_VAR"					  VALID
	run_test "unset valid"						 "unset VAR"						  VALID
	run_test "unset multiple"					  "unset A B C"						VALID
	run_test "cd no args"						  "cd"								 VALID
	run_test "cd with path"						"cd /tmp"							VALID
	run_test "exit no args"						"exit"							   VALID
	run_test "exit with numeric arg"			   "exit 42"							VALID

	# Builtin runtime/error cases: parser is valid, but builtin should complain.
	run_test "export invalid starts with number"	"export 3hi"						 BUILTIN_ERR "not an identifier|not a valid identifier"
	run_test "export invalid empty key"			 "export =value"					  BUILTIN_ERR "not an identifier|not a valid identifier"
	run_test "export invalid contains dash"		 "export BAD-NAME=value"			  BUILTIN_ERR "not an identifier|not a valid identifier"
	run_test "unset invalid starts with number"	 "unset 3hi"						  BUILTIN_ERR "not an identifier|not a valid identifier"
	run_test "cd too many args"					 "cd /tmp /"						  BUILTIN_ERR "too many arguments"
	run_test "cd nonexistent path"				  "cd /path/that/does/not/exist"	   BUILTIN_ERR "No such file|cd:"
	run_test "exit non numeric"					 "exit abc"						   BUILTIN_ERR "numeric argument required"
}

main() {
	print_header
	compile_if_needed
	TMP_DIR=$(mktemp -d)

	echo -e "${GRAY}Mode:${RESET} $([[ "$STRICT_EXIT" -eq 1 ]] && echo "strict exit code" || echo "development mode")"
	echo -e "${GRAY}Tip:${RESET} run './mandatory_parsing_builtin_tester.sh --strict-exit' later when main returns real exit statuses."

	run_valid_parsing_tests
	run_syntax_error_tests
	run_unsupported_mandatory_tests
	run_builtin_tests

	echo ""
	echo -e "${BOLD}${BLUE}============================================================${RESET}"
	echo -e "${BOLD}TOTAL:${RESET} $TOTAL   ${GREEN}PASS:${RESET} $PASS   ${RED}FAIL:${RESET} $FAIL"
	echo -e "${BOLD}${BLUE}============================================================${RESET}"

	if [[ "$FAIL" -eq 0 ]]; then
		exit 0
	fi
	exit 1
}

main "$@"
