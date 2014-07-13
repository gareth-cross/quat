/*
 * quaternion.hpp
 *
 *  Copyright (c) 2014 Gareth Cross. MIT License.
 *
 *  This file is part of quaternion.
 *
 *  Created on: 13/06/2014
 *     Authors: Gareth Cross & Chao Qu
 */
#include "quaternion.hpp"

#include <stdio.h>
#include <gtest/gtest.h>

//   use eigen for unit tests
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace extra;

#define DOUBLE_NEAR_THRESH (1e-12)

// Setting up test fixture
class QuatTest : public testing::Test {
 protected:
  QuatTest()
      : q1d_(1.0, 1.0, 1.0, 1.0),
        q2d_(0.533215448243828, 0.592817248117098, 0.083109566226999,
             0.597780725760344) {

    // Matches q2d in SO(3), do not change
    q2d_mtx_(0, 0) = 0.271502007821992;
    q2d_mtx_(0, 1) = -0.538954266589856;
    q2d_mtx_(0, 2) = 0.797380058863537;
    q2d_mtx_(1, 0) = 0.736029403961437;
    q2d_mtx_(1, 1) = -0.417548171511386;
    q2d_mtx_(1, 2) = -0.532836035729257;
    q2d_mtx_(2, 0) = 0.620118840427219;
    q2d_mtx_(2, 1) = 0.731561222996468;
    q2d_mtx_(2, 2) = 0.283321020672862;
  }
  // virtual void SetUp() {}
  // virtual void TearDown() {}
  quat<double> q0d_;
  quat<double> q1d_;

  quat<double> q2d_;
  Eigen::Matrix3d q2d_mtx_;
};  // class QuatTest

TEST(quat, DifferentType) {
  const quatf q(0.1, 0.1, 0.1, 0.1);
  EXPECT_NE(0.1, q.a());  // float and double should be different
}

TEST_F(QuatTest, DefaultConstructor) {
  EXPECT_EQ(1.0, q0d_.a());
  EXPECT_EQ(0.0, q0d_.b());
  EXPECT_EQ(0.0, q0d_.c());
  EXPECT_EQ(0.0, q0d_.d());
}

TEST_F(QuatTest, ConstructorFromScalar) {
  EXPECT_EQ(1.0, q1d_.a());
  EXPECT_EQ(1.0, q1d_.b());
  EXPECT_EQ(1.0, q1d_.c());
  EXPECT_EQ(1.0, q1d_.d());
  quat<double> q(0.1, 0.1, 0.1, 0.1);
  EXPECT_EQ(0.1, q.a());
}

TEST_F(QuatTest, CopyConstructor) {
  const quat<double> q(q1d_);
  EXPECT_EQ(q1d_.a(), q.a());
  EXPECT_EQ(q1d_.b(), q.b());
  EXPECT_EQ(q1d_.c(), q.c());
  EXPECT_EQ(q1d_.d(), q.d());
}

TEST_F(QuatTest, Normalize) {
  q1d_.normalize();
  EXPECT_EQ(0.5, q1d_.a());
  EXPECT_EQ(0.5, q1d_.b());
  EXPECT_EQ(0.5, q1d_.c());
  EXPECT_EQ(0.5, q1d_.d());
}

TEST_F(QuatTest, Norm) { EXPECT_EQ(2.0, q1d_.norm()); }

TEST_F(QuatTest, Conjugate) {
  const quatd q = q1d_.conjugate();
  EXPECT_EQ(1.0, q.a());
  EXPECT_EQ(-1.0, q.b());
  EXPECT_EQ(-1.0, q.c());
  EXPECT_EQ(-1.0, q.d());
}

TEST_F(QuatTest, ToMatrix) {
  Eigen::Matrix3d R = q2d_.toMatrix<Eigen::Matrix3d>();
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      EXPECT_NEAR(R(i, j), q2d_mtx_(i, j), DOUBLE_NEAR_THRESH);
    }
  }
}

TEST_F(QuatTest, FromMatrix) {
  quat<double> qtest = quat<double>::fromMatrix(q2d_mtx_);
  for (int i = 0; i < 4; i++) {
    EXPECT_NEAR(qtest(i), q2d_(i), DOUBLE_NEAR_THRESH);
  }
}

// Test the following tricky matrices:
//
// 0  1  0  case 1 and 4 invalid
// 1  0  0
// 0  0 -1
//
// -1  0  0 case 2 invalid
//  0  0  1
//  0  1  0
//
//  0  0  1 case 3 invalid
//  0 -1  0
//  1  0  0
TEST(quat, FromMatrixSingularities) {
  Eigen::Matrix3d S;
  S.setZero();
  S(0, 1) = 1;
  S(1, 0) = 1;
  S(2, 2) = -1;

  quat<double> qtest = quat<double>::fromMatrix(S);
  EXPECT_NEAR(qtest(0), 0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(1), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(2), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(3), 0, DOUBLE_NEAR_THRESH);

  S.setZero();
  S(0, 0) = -1;
  S(1, 2) = 1;
  S(2, 1) = 1;

  qtest = quat<double>::fromMatrix(S);
  EXPECT_NEAR(qtest(0), 0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(1), 0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(2), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(3), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);

  S.setZero();
  S(0, 2) = 1;
  S(1, 1) = -1;
  S(2, 0) = 1;

  qtest = quat<double>::fromMatrix(S);
  EXPECT_NEAR(qtest(0), 0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(1), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(2), 0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qtest(3), 1 / std::sqrt(2), DOUBLE_NEAR_THRESH);
}

TEST(quat, Addition) {
  const quat<double> qa(1.0, 2.0, 3.0, 4.0);
  const quat<double> qb(1.0, -2.0, 3.0, -2.0);
  const quat<double> qc = qa+qb;

  EXPECT_NEAR(qc(0), 2.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(1), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(2), 6.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(3), 2.0, DOUBLE_NEAR_THRESH);
}

TEST(quat, Multiply) {
  const quat<double> qa(1/std::sqrt(2.0), 1/std::sqrt(2.0), 0.0, 0.0);
  const quat<double> qb(1/std::sqrt(2.0), 0.0, 1/std::sqrt(2.0), 0.0);
  quat<double> qc = qa * qb;

  EXPECT_NEAR(qc(0), 0.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(1), 0.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(2), 0.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(3), 0.5, DOUBLE_NEAR_THRESH);

  qc = qa * qa.conjugate();

  EXPECT_NEAR(qc(0), 1.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(1), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(2), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qc(3), 0.0, DOUBLE_NEAR_THRESH);
}

TEST(quat, Scaling) {
  quat<double> q(1.0, 2.0, 3.0, 4.0);
  q = q * 2.0;

  EXPECT_NEAR(q(0), 2.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(1), 4.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(2), 6.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(3), 8.0, DOUBLE_NEAR_THRESH);

  q *= 0.5;

  EXPECT_NEAR(q(0), 1.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(1), 2.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(2), 3.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(3), 4.0, DOUBLE_NEAR_THRESH);

  q /= 2.0;

  EXPECT_NEAR(q(0), 0.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(1), 1.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(2), 1.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(q(3), 2.0, DOUBLE_NEAR_THRESH);
}

TEST(quat, Rotation) {
  const quat<double> qx = quat<double>::rotation(M_PI/3, 1.0, 0.0, 0.0);

  EXPECT_NEAR(qx(0), std::sqrt(3.0)/2, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qx(1), 0.5, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qx(2), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qx(3), 0.0, DOUBLE_NEAR_THRESH);

  const quat<double> qy = quat<double>::rotation(M_PI/2, 0.0, 1.0, 0.0);

  EXPECT_NEAR(qy(0), 1/std::sqrt(2.0), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qy(1), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qy(2), 1/std::sqrt(2.0), DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qy(3), 0.0, DOUBLE_NEAR_THRESH);

  const quat<double> qz = quat<double>::rotation(M_PI, 0.0, 0.0, 1.0);

  EXPECT_NEAR(qz(0), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qz(1), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qz(2), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qz(3), 1.0, DOUBLE_NEAR_THRESH);

  const quat<double> qnull = quat<double>::rotation(0,0,0);

  EXPECT_NEAR(qnull(0), 1.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qnull(1), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qnull(2), 0.0, DOUBLE_NEAR_THRESH);
  EXPECT_NEAR(qnull(3), 0.0, DOUBLE_NEAR_THRESH);
}

TEST(quat, Integration) {

  quat<double> qe, qr;
  quat<double> w(0.0, 0.0, 0.0, 60.0 / 180.0 * M_PI);

  for (int i=0; i < 100000; i++) {
    qe.integrateEuler(w, 0.00001);
    qr.integrateRungeKutta4(w, 0.00001);
  }

  // use a more relaxed threshold for these
  EXPECT_NEAR(qe(0), std::sqrt(3.0)/2, 1e-10);
  EXPECT_NEAR(qe(1), 0.0, 1e-10);
  EXPECT_NEAR(qe(2), 0.0, 1e-10);
  EXPECT_NEAR(qe(3), 0.5, 1e-10);

  EXPECT_NEAR(qr(0), std::sqrt(3.0)/2, 1e-10);
  EXPECT_NEAR(qr(1), 0.0, 1e-10);
  EXPECT_NEAR(qr(2), 0.0, 1e-10);
  EXPECT_NEAR(qr(3), 0.5, 1e-10);
}
