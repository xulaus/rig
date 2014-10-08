Rig is a small program for the calculation of Elo ratings and the use of these to predict the outcomes.
Whilst it is geared toward tennis data, it should be general enough to use on all sorts of data.
It is named after the Norse god who was said to be able to see the future.

Usage
===
Simple calculation of elo ratings from a list of outcomes

    rig -i input_file
This list data must be formatted either with outcomes as

    Player One 2 - 1 Player Two
Or as just the names

    Winner - Loser
    
Tennis results are often distributed as tree like data rather than as a list of outcomes.
Rig can deal with this data directly as

    rig --itree -i input_file
Or it can transform this data to list format (useful for building large datasets)

    rig -itree -i input_file --olist -o output_file
Finally, the expected winner out of two players can be calculated via 

    rig -i input_file --ocomp "A. Murray" "S. Johnson"

Building Rig
===
Rig has been built successfully with Clang and GCC but older versions of GCC may fail to work correctly due to an incomplete `libstdc++`.
To check if rig will work correctly run

	make test
	./rig_test

In windows `cmd` you will need to run `rig_test` instead of `./rig_test`.
To then build rig, run

	make
