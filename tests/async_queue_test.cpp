#include <catch2/catch_test_macros.hpp>
#include <thread>
#include <queue>

#include "async_queue.hpp"

/**
 * \test AsyncQueue Test
 * @brief
 * Please make sure to read async_queue documentation to know how to use it
 */

//Before proceding we assure that the basic functionality of our queue is working
TEST_CASE( "Base async_queue test", "[async_queue]" ) {

    AsyncQueue as_queue;
    std::string value{"Test 1234"};
    as_queue.setValue(value);
    REQUIRE( as_queue.getValue() == value );

    SECTION("Normal usage"){
        value = "Drawed Dark Knigt @1234";
        as_queue.setValue(value);
        REQUIRE( as_queue.getValue() == value );
        as_queue.setValue(value);
        REQUIRE( as_queue.getValue() == value );
        value = "!@#$%^&*()_+-= Summoned Goblin, 3 to 3, No Attack";
        as_queue.setValue(value);
        REQUIRE( as_queue.getValue() == value );
    }

    SECTION("Using an empty string"){
        value = "";
        as_queue.setValue(value);
        REQUIRE( as_queue.getValue() == value );
    }

}

TEST_CASE( "Testing the asynchronous part", "[async test]" ) {

    AsyncQueue main_queue;
    AsyncQueue thread_queue;
    main_queue.setValue("Thread");
    thread_queue.setValue("Thread");
    REQUIRE(main_queue.getValue() == thread_queue.getValue());

    SECTION("Asynchronous test"){
        std::thread my_thread(
            [&](){
                std::string value = "Thread test 1";
                main_queue.setValue(value);
                thread_queue.setValue(value);
                value = "Thread test 2";
                thread_queue.setValue(value);
                main_queue.setValue(value);
                
            }
        );
        REQUIRE( thread_queue.getValue() == main_queue.getValue() );
        REQUIRE( thread_queue.getValue() == main_queue.getValue() );

        my_thread.join();
    }

    SECTION("Testing intertwined functionality"){
        std::thread my_thread(
            [&](){
                thread_queue.setValue(main_queue.getValue());
                thread_queue.setValue(main_queue.getValue());
            }
        );
        std::string value = "Main test 1";
        main_queue.setValue(value);
        REQUIRE( value == thread_queue.getValue() );
        value = "Main test 2";
        main_queue.setValue(value);
        REQUIRE( thread_queue.getValue() == value);

        my_thread.join();
    }

    SECTION("Testing queue functionality in multiple threads"){
        std::queue<std::string> my_queue;
        my_queue.emplace("First value");
        my_queue.emplace("Second value");
        my_queue.emplace("Third value");
        my_queue.emplace("Fourth value");
        my_queue.emplace("Fifth value");

        main_queue.setValue("First value");
        main_queue.setValue("Second value");
        

        std::thread my_thread(
            [&](){
                main_queue.setValue("Third value");
                main_queue.setValue("Fourth value");
                main_queue.setValue("Fifth value");
                
            }
        );
        while(!my_queue.empty()){
            REQUIRE( my_queue.front() == main_queue.getValue() );
            my_queue.pop();
        }
        main_queue.setValue("Sixth value");
        REQUIRE( "Sixth value" == main_queue.getValue() );

        my_thread.join();
    }
}