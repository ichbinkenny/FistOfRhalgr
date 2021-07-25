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
    network.get_participant_dps("TEST USER", found);
    EXPECT_FALSE(network.start());
    EXPECT_EQ("Idle", network.get_operation_mode()); 
    EXPECT_FALSE(found);
    bool running = network.is_active();
    EXPECT_FALSE(running);
  };
} // end anonymous namespace
