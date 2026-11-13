"""Compile-only comparator migration checks. Run: python3 test_comparator_headers.py"""
from pathlib import Path
import os
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parent
BST = '#include "gen_BST/gen.bst.h"\n'
HEAP = '#include "gen_heap/gen_heap.h"\n'
CASES = [
 ('BST then heap',BST+HEAP+'BSTComparator b; HeapComparator h;\n',True),
 ('heap then BST',HEAP+BST+'BSTComparator b; HeapComparator h;\n',True),
 ('BST legacy alias','#define BST_ENABLE_LEGACY_COMPARATOR\n'+BST+'Comparator b;\n',True),
 ('heap legacy alias','#define HEAP_ENABLE_LEGACY_COMPARATOR\n'+HEAP+'Comparator h;\n',True),
 ('both headers with BST alias','#define BST_ENABLE_LEGACY_COMPARATOR\n'+BST+HEAP+'Comparator b; HeapComparator h;\n',True),
 ('conflicting aliases','#define BST_ENABLE_LEGACY_COMPARATOR\n#define HEAP_ENABLE_LEGACY_COMPARATOR\n'+BST+HEAP,False),
]
with tempfile.TemporaryDirectory() as directory:
 source=Path(directory)/'check.c'
 for name,code,expected in CASES:
  source.write_text(code)
  result=subprocess.run([os.environ.get('CC','cc'),'-std=c11','-Wall','-Wextra','-Werror','-I'+str(ROOT),'-fsyntax-only',str(source)],capture_output=True,text=True)
  assert (result.returncode==0)==expected,(name,result.stderr)
  if not expected:
   assert 'Only one legacy Comparator alias' in result.stderr,result.stderr
  print('PASS:',name)
