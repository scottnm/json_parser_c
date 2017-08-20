test_set = [
               ("test/jsons/err_improper_obj_opening.json", "Error, bad obj syntax", 1),
               ("test/jsons/err_key_missing_opening_quote.json", "When trying to parse a string, a value other than an opening quote was found", 1),
               ("test/jsons/err_key_missing_colon.json", "Missing ':' after key", 1),
               ("test/jsons/err_unexpected_EOF.json", "Unexpected EOF", 1),
               ("test/jsons/err_improper_num.json", "Invalid number could not be parsed", 1),
               ("test/jsons/err_invalid_lookahead.json", "invalid character parsed", 1),
               ("test/jsons/err_keyvalue_pair_unclosed.json", "End of keyvalue pair was not followed by a comma or a closing brace", 1)
           ];
