#!/usr/bin/env -S v

import os

const repo_root = getwd()
const tests_dir = join_path(repo_root, 'integration_tests')

fn proceed_tests(path string) ! {
	mut entries := ls(path)!
	for mut entry in entries {
		full := join_path(path, entry)
		if is_dir(full) {
			proceed_tests(full)!
		}

		if entry.ends_with('.pul') {
			println('\n\n=> ${entry}')
			print(execute('${repo_root}/build/main -ptok ${full}').output)
		}
	}
}

args := os.args
if args.len > 1 && args[1] == '-I' {
	for name in args[2..] {
		print(execute('${repo_root}/build/main -ptok ${join_path(tests_dir, name)}').output)
	}
} else {
	proceed_tests(tests_dir)!
}
