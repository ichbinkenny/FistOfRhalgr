#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../RhalgrNetwork.hpp"

namespace 
{
  class RhalgrNetworkTest : public ::testing::Test
  {
    protected:
      virtual void SetUp()
      {

      }

      virtual void TearDown()
      {

      }
  };

  TEST_F(RhalgrNetworkTest, initialization_test)
  {
    RhalgrNetwork network;
    bool found;
    EXPECT_EQ(network.get_interval(), 1.0);
    EXPECT_EQ(network.get_participants().size(), 0);
    network.get_participant_dps("TEST USER", &found);
    EXPECT_FALSE(network.start());
    EXPECT_EQ("Idle", network.get_operation_mode()); 
    EXPECT_FALSE(found);
    bool running = network.is_active();
    EXPECT_FALSE(running);
  };

  TEST_F(RhalgrNetworkTest, start_empty_if_name_errors)
  {
    RhalgrNetwork network;
    EXPECT_FALSE(network.start());
  };

  TEST_F(RhalgrNetworkTest, start_nonexistent_if_name_errors)
  {
    RhalgrNetwork network("a really long and fake if name");
    EXPECT_FALSE(network.start());
  };

  TEST_F(RhalgrNetworkTest, set_interval_negative_value_fails)
  {
    RhalgrNetwork network;
    EXPECT_FALSE(network.set_parse_interval(-1.0));
    EXPECT_EQ(network.get_interval(), 1.0);
  };

  TEST_F(RhalgrNetworkTest, set_interval_to_zero_fails)
  {
    RhalgrNetwork network;
    EXPECT_FALSE(network.set_parse_interval(0.0));
    EXPECT_EQ(network.get_interval(), 1.0);
  };

  TEST_F(RhalgrNetworkTest, set_interval_valid_time_updates)
  {
    RhalgrNetwork network;
    EXPECT_TRUE(network.set_parse_interval(2.345));
    EXPECT_EQ(network.get_interval(), 2.345);
  };

  TEST_F(RhalgrNetworkTest, get_participant_dps_empty_data_returns_false_found)
  {
    RhalgrNetwork network;
    bool found = true;
    network.get_participant_dps("FAKE AND WAYYYYYYYYYY TO LONG USER NAME HERE", &found);
    EXPECT_FALSE(found);
  };

  TEST_F(RhalgrNetworkTest, get_participants_size_zero_initial)
  {
    RhalgrNetwork network;
    EXPECT_EQ(network.get_participants().size(), 0);
  };

  TEST_F(RhalgrNetworkTest, mode_string_changes_on_run_start)
  {
    RhalgrNetwork network("enp3s0");
    ASSERT_TRUE(network.start());
    EXPECT_EQ(network.get_operation_mode(), "Active");
  };

  TEST_F(RhalgrNetworkTest, mode_string_toggle_on_run_end)
  {
    RhalgrNetwork network("enp3s0");
    ASSERT_TRUE(network.start());
    EXPECT_EQ(network.get_operation_mode(), "Active");
    network.stop();
    EXPECT_EQ(network.get_operation_mode(), "Halted");
  };

  TEST_F(RhalgrNetworkTest, running_true_on_successful_run_start)
  {
    RhalgrNetwork network("enp3s0");
    ASSERT_TRUE(network.start());
    EXPECT_TRUE(network.is_active());
  };

  TEST_F(RhalgrNetworkTest, running_false_after_stopping_successful_run)
  {
    RhalgrNetwork network("enp3s0");
    ASSERT_TRUE(network.start());
    EXPECT_TRUE(network.is_active());
    network.stop();
    EXPECT_FALSE(network.is_active());
  };
} // end anonymous namespace
