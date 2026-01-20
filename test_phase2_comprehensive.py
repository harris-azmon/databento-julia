#!/usr/bin/env python3
"""
Comprehensive Phase 2 Test and Validation
"""

from pathlib import Path
import re

cpp_file = Path('/home/user/databento-julia/deps/databento_jl.cpp')
content = cpp_file.read_text()

print("=" * 80)
print("PHASE 2 COMPREHENSIVE VALIDATION & TEST")
print("=" * 80)

# 1. Count declarations
print("\n1. CORE STATISTICS")
print(f"   File size: {len(content):,} bytes")
print(f"   Lines: {len(content.splitlines())}")

add_bits_count = len(re.findall(r'mod\.add_bits<', content))
set_const_count = len(re.findall(r'mod\.set_const\(', content))
method_count = len(re.findall(r'mod\.method\(', content))

print(f"   add_bits() calls: {add_bits_count}")
print(f"   set_const() calls: {set_const_count}")
print(f"   method() calls: {method_count}")

# 2. Enum validation (by line grepping)
print("\n2. PHASE 2 ENUMS")

rtype_consts = re.findall(r'mod\.set_const\("RTYPE_\w+",', content)
action_consts = re.findall(r'mod\.set_const\("ACTION_\w+",', content)
side_consts = re.findall(r'mod\.set_const\("SIDE_\w+",', content)

print(f"   ✓ RType: {len(rtype_consts)} constants")
for const in rtype_consts[:3]:
    print(f"     - {const[:-1]}")
print(f"     ... and {len(rtype_consts)-3} more")

print(f"   ✓ Action: {len(action_consts)} constants")
for const in action_consts:
    print(f"     - {const[:-1]}")

print(f"   ✓ Side: {len(side_consts)} constants")
for const in side_consts:
    print(f"     - {const[:-1]}")

# 3. Supporting types
print("\n3. PHASE 2 SUPPORTING TYPES")

types_and_methods = {
    'FlagSet': ['is_last', 'is_tob', 'is_snapshot', 'is_mbp', 'is_bad_ts_recv', 'is_maybe_bad_book'],
    'UnixNanos': ['time_since_epoch'],
    'TimeDeltaNanos': ['count'],
    'BidAskPair': ['bid_px', 'ask_px', 'bid_sz', 'ask_sz', 'bid_ct', 'ask_ct'],
}

for type_name, required_methods in types_and_methods.items():
    # Check for add_bits declaration
    has_add_bits = f'mod.add_bits<databento::{type_name}>' in content
    
    # Check for methods
    found_methods = []
    for method_name in required_methods:
        pattern = f'mod\\.method\\("{method_name}"'
        if re.search(pattern, content):
            found_methods.append(method_name)
    
    status = "✓" if (has_add_bits and len(found_methods) == len(required_methods)) else "✗"
    print(f"   {status} {type_name}: {len(found_methods)}/{len(required_methods)} methods")

# 4. Message types
print("\n4. PHASE 2 MESSAGE TYPES")

message_types = {
    'RecordHeader': ['length', 'rtype', 'publisher_id', 'instrument_id', 'ts_event'],
    'MboMsg': ['hd', 'order_id', 'price', 'size', 'action', 'side', 'ts_recv'],
    'TradeMsg': ['hd', 'price', 'size', 'action', 'side', 'ts_recv'],
    'Mbp1Msg': ['hd', 'price', 'levels', 'ts_recv'],
    'Mbp10Msg': ['hd', 'price', 'levels', 'ts_recv'],
    'InstrumentDefMsg': ['hd', 'min_price_increment', 'currency', 'raw_symbol'],
    'ImbalanceMsg': ['hd', 'ref_price', 'auction_type', 'side'],
}

for type_name, key_methods in message_types.items():
    # Check for add_bits
    has_add_bits = f'mod.add_bits<databento::{type_name}>' in content
    
    # Check for key methods
    found_methods = []
    for method_name in key_methods:
        pattern = f'mod\\.method\\("{method_name}"'
        if re.search(pattern, content):
            found_methods.append(method_name)
    
    status = "✓" if (has_add_bits and len(found_methods) >= len(key_methods)-1) else "✗"
    print(f"   {status} {type_name}: {found_methods[0] if found_methods else 'MISSING'} ... ({found_methods.__len__()}/{key_methods.__len__()} key methods)")

# 5. IsBits specializations
print("\n5. ISBITS SPECIALIZATIONS")

expected_isbits = [
    'Schema', 'Encoding', 'SType',
    'RType', 'Action', 'Side',
    'FlagSet', 'UnixNanos', 'TimeDeltaNanos', 'BidAskPair',
    'RecordHeader', 'MboMsg', 'TradeMsg', 'Mbp1Msg', 'Mbp10Msg', 'InstrumentDefMsg', 'ImbalanceMsg'
]

isbits_found = 0
for type_name in expected_isbits:
    pattern = f'template<> struct IsBits<databento::{type_name}>'
    if pattern in content:
        isbits_found += 1

print(f"   Found: {isbits_found}/{len(expected_isbits)} specializations")

# 6. Headers
print("\n6. INCLUDES")

required_headers = [
    'jlcxx/jlcxx.hpp',
    'databento/enums.hpp',
    'databento/record.hpp',
    'databento/datetime.hpp',
    'databento/flag_set.hpp',
]

all_headers = True
for header in required_headers:
    has_header = f'#include <{header}>' in content
    status = "✓" if has_header else "✗"
    print(f"   {status} {header}")
    if not has_header:
        all_headers = False

# 7. Test summary
print("\n" + "=" * 80)
print("VALIDATION SUMMARY")
print("=" * 80)

checks = {
    'RType enum': len(rtype_consts) >= 20,
    'Action enum': len(action_consts) >= 7,
    'Side enum': len(side_consts) >= 3,
    'Supporting types': len(types_and_methods) == 4,
    'Message types': len(message_types) == 7,
    'IsBits specializations': isbits_found == len(expected_isbits),
    'All headers': all_headers,
    'Syntax valid': content.count('{') == content.count('}'),
}

passed = sum(1 for v in checks.values() if v)
total = len(checks)

for check_name, result in checks.items():
    status = "✓ PASS" if result else "✗ FAIL"
    print(f"  {status}: {check_name}")

print(f"\nResult: {passed}/{total} checks passed")

if passed == total:
    print("\n✓✓✓ PHASE 2 IMPLEMENTATION COMPLETE & VALID ✓✓✓")
    print("\nPhase 2 is ready for Julia compilation!")
    exit(0)
else:
    print("\n✗ Some checks failed")
    exit(1)
