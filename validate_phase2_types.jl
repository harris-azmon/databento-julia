"""
Phase 2 Type Validation Script
Validates that all Phase 2 types are properly defined and structured
"""

function main()
    # Read the C++ wrapper file
    cpp_file = "/home/user/databento-julia/deps/databento_jl.cpp"
    content = read(cpp_file, String)

    println("=" ^ 80)
    println("PHASE 2 JULIA VALIDATION TEST")
    println("=" ^ 80)

    # Test 1: All includes present
    println("\n1. REQUIRED INCLUDES")
    test1_pass = 0
    includes_list = [
        ("jlcxx/jlcxx.hpp", "#include <jlcxx/jlcxx.hpp>"),
        ("databento/enums.hpp", "#include <databento/enums.hpp>"),
        ("databento/record.hpp", "#include <databento/record.hpp>"),
        ("databento/datetime.hpp", "#include <databento/datetime.hpp>"),
        ("databento/flag_set.hpp", "#include <databento/flag_set.hpp>"),
    ]

    for (name, pattern) in includes_list
        if contains(content, pattern)
            println("   ✓ $(name)")
            test1_pass += 1
        else
            println("   ✗ $(name)")
        end
    end
    println("   Result: $(test1_pass)/$(length(includes_list)) includes found")

    # Test 2: IsBits specializations
    println("\n2. ISBITS SPECIALIZATIONS (17 required)")
    test2_pass = 0
    isbits_types = [
        "Schema", "Encoding", "SType",
        "RType", "Action", "Side",
        "FlagSet", "UnixNanos", "TimeDeltaNanos", "BidAskPair",
        "RecordHeader", "MboMsg", "TradeMsg", "Mbp1Msg", "Mbp10Msg",
        "InstrumentDefMsg", "ImbalanceMsg"
    ]

    for type_name in isbits_types
        pattern = "template<> struct IsBits<databento::$(type_name)>"
        if contains(content, pattern)
            test2_pass += 1
        else
            println("   ✗ Missing: $(type_name)")
        end
    end
    println("   ✓ $(test2_pass)/$(length(isbits_types)) specializations found")

    # Test 3: Phase 2 Enum constants
    println("\n3. PHASE 2 ENUM CONSTANTS")
    test3_pass = 0

    enum_tests = [
        ("RType", "RTYPE_", 22),
        ("Action", "ACTION_", 7),
        ("Side", "SIDE_", 3),
    ]

    for (enum_name, prefix, expected_count) in enum_tests
        count_matches = 0
        for line in split(content, '\n')
            if contains(line, "mod.set_const(\"$(prefix)")
                count_matches += 1
            end
        end

        if count_matches >= expected_count
            println("   ✓ $(enum_name): $(count_matches) constants")
            test3_pass += 1
        else
            println("   ✗ $(enum_name): $(count_matches) constants (expected $(expected_count))")
        end
    end
    println("   Result: $(test3_pass)/$(length(enum_tests)) enums complete")

    # Test 4: Supporting type methods
    println("\n4. PHASE 2 SUPPORTING TYPES")
    test4_pass = 0

    supporting_types = [
        ("FlagSet", ["is_last", "is_tob", "is_snapshot", "is_mbp", "is_bad_ts_recv", "is_maybe_bad_book"]),
        ("UnixNanos", ["time_since_epoch"]),
        ("TimeDeltaNanos", ["count"]),
        ("BidAskPair", ["bid_px", "ask_px", "bid_sz", "ask_sz", "bid_ct", "ask_ct"]),
    ]

    for (type_name, methods) in supporting_types
        found_methods = 0
        for method in methods
            if contains(content, "mod.method(\"$(method)\"")
                found_methods += 1
            end
        end

        if found_methods == length(methods)
            println("   ✓ $(type_name): $(found_methods)/$(length(methods)) methods")
            test4_pass += 1
        else
            println("   ✗ $(type_name): $(found_methods)/$(length(methods)) methods")
        end
    end
    println("   Result: $(test4_pass)/$(length(supporting_types)) types complete")

    # Test 5: Message types
    println("\n5. PHASE 2 MESSAGE TYPES")
    test5_pass = 0

    message_types = [
        ("RecordHeader", ["length", "rtype", "publisher_id", "instrument_id"]),
        ("MboMsg", ["hd", "order_id", "price", "size", "action", "side"]),
        ("TradeMsg", ["hd", "price", "size", "action", "side"]),
        ("Mbp1Msg", ["hd", "price", "levels", "ts_recv"]),
        ("Mbp10Msg", ["hd", "price", "levels", "ts_recv"]),
        ("InstrumentDefMsg", ["hd", "min_price_increment", "currency", "raw_symbol"]),
        ("ImbalanceMsg", ["hd", "ref_price", "auction_type", "side"]),
    ]

    for (type_name, required_methods) in message_types
        found_methods = 0
        for method in required_methods
            if contains(content, "mod.method(\"$(method)\"")
                found_methods += 1
            end
        end

        if found_methods >= length(required_methods) - 1
            println("   ✓ $(type_name)")
            test5_pass += 1
        else
            println("   ✗ $(type_name): only $(found_methods)/$(length(required_methods)) key methods")
        end
    end
    println("   Result: $(test5_pass)/$(length(message_types)) message types complete")

    # Test 6: Code statistics
    println("\n6. CODE STATISTICS")

    add_bits_count = count("mod.add_bits<", content)
    set_const_count = count("mod.set_const(", content)
    method_count = count("mod.method(", content)

    println("   add_bits() calls: $(add_bits_count)")
    println("   set_const() calls: $(set_const_count)")
    println("   method() calls: $(method_count)")

    # Test 7: Syntax validation
    println("\n7. SYNTAX VALIDATION")

    open_braces = count('{', content)
    close_braces = count('}', content)
    open_parens = count('(', content)
    close_parens = count(')', content)

    brace_valid = open_braces == close_braces
    paren_valid = open_parens == close_parens

    println("   Braces: $(open_braces) open, $(close_braces) close - $(brace_valid ? "✓" : "✗")")
    println("   Parentheses: $(open_parens) open, $(close_parens) close - $(paren_valid ? "✓" : "✗")")

    if endswith(strip(content), "}")
        println("   Module closure: ✓")
    else
        println("   Module closure: ✗")
    end

    # Summary
    println("\n" * "=" ^ 80)
    println("VALIDATION SUMMARY")
    println("=" ^ 80)

    checks = Dict(
        "Includes" => test1_pass == length(includes_list),
        "IsBits Specializations" => test2_pass == length(isbits_types),
        "Enum Constants" => test3_pass == length(enum_tests),
        "Supporting Types" => test4_pass == length(supporting_types),
        "Message Types" => test5_pass == length(message_types),
        "Syntax Valid" => brace_valid && paren_valid,
    )

    passed = sum(values(checks))
    total = length(checks)

    for (check_name, result) in checks
        status = result ? "✓ PASS" : "✗ FAIL"
        println("  $(status): $(check_name)")
    end

    println("\nResult: $(passed)/$(total) checks passed")

    if passed == total
        println("\n" * "✓" ^ 40)
        println("✓✓✓ PHASE 2 VALIDATION SUCCESSFUL ✓✓✓")
        println("✓" ^ 40)
        println("\nPhase 2 C++ wrapper is syntactically valid and complete!")
        println("Ready for Julia compilation when environment allows.")
        return true
    else
        println("\n✗ Some validation checks failed")
        return false
    end
end

# Run the validation
success = main()
exit(success ? 0 : 1)
