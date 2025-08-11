#include "gates.hpp"
#include <gtest/gtest.h>

TEST(OrGate, TruthTable)
{
    gates::OrGate og;

    og.Inputs().SetA(true);
    og.Inputs().SetB(true);
    EXPECT_TRUE(og.Output());

    og.Inputs().SetA(true);
    og.Inputs().SetB(false);
    EXPECT_TRUE(og.Output());

    og.Inputs().SetA(false);
    og.Inputs().SetB(true);
    EXPECT_TRUE(og.Output());

    og.Inputs().SetA(false);
    og.Inputs().SetB(false);
    EXPECT_FALSE(og.Output());
}

TEST(AndGate, TruthTable)
{
    gates::AndGate ag;

    ag.Inputs().SetA(true);
    ag.Inputs().SetB(true);
    EXPECT_TRUE(ag.Output());

    ag.Inputs().SetA(true);
    ag.Inputs().SetB(false);
    EXPECT_FALSE(ag.Output());

    ag.Inputs().SetA(false);
    ag.Inputs().SetB(true);
    EXPECT_FALSE(ag.Output());

    ag.Inputs().SetA(false);
    ag.Inputs().SetB(false);
    EXPECT_FALSE(ag.Output());
}
