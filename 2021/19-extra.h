#if 0
void read_dummy_points() {
  v3 TEST_POINTS[16] = {
    {-15.0f, 1.0f, -2.0f},
    {-15.0f, 1.0f, -22.0f},
    {-455.0f, 111.0f, -215.0f},
    {-5.0f, 1.0f, -2.0f},
    {0.0f, -2.0f, 3.0f},
    {0.0f, -2.0f, 33.0f},
    {0.0f, -25.0f, 3.0f},
    {1.0f, 2.0f, 3.0f},
    {10.0f, 20.0f, 30.0f},
    {1.0f, 2.0f, 33.0f},
    {125.0f, 212.0f, 17.0f},
    {13.0f, 25.0f, 1.0f},
    {171.0f, 172.0f, 293.0f},
    {20.0f, -222.0f, 53.0f},
    {3.0f, 12.0f, 1.0f},
    {3.0f, 2.0f, 1.0f},
  };

  n_scanners = 2;

  for (i32 idx_scanner = 0; idx_scanner < n_scanners; idx_scanner++) {
    if (idx_scanner == 0) {
      n_points[idx_scanner] = 16;
    } else {
      n_points[idx_scanner] = 12;
    }
    if (idx_scanner == 0) {
      for (i32 idx_point = 0; idx_point < n_points[idx_scanner]; idx_point++) {
        points[idx_scanner][idx_point][0] = TEST_POINTS[idx_point][0];
        points[idx_scanner][idx_point][1] = TEST_POINTS[idx_point][1];
        points[idx_scanner][idx_point][2] = TEST_POINTS[idx_point][2];
      }
    } else {
      for (i32 idx_point = 0; idx_point < n_points[idx_scanner]; idx_point++) {
        m3 *transform = &cob_matrices[idx_scanner - 1];
        m3 inv_transform;
        glm_mat3_transpose_to(*transform, inv_transform);
        v3 transformed_point;
        glm_mat3_mulv(inv_transform, TEST_POINTS[idx_point], points[idx_scanner][idx_point]);
        points[idx_scanner][idx_point][0] += idx_scanner;
        points[idx_scanner][idx_point][1] -= idx_scanner * 2.0f;
        points[idx_scanner][idx_point][2] += idx_scanner * 3.0f;
      }
    }
  }
}


void print_scanners_and_points() {
  printf("%d scanners\n\n", n_scanners);
  for (i32 idx_scanner = 0; idx_scanner < n_scanners; idx_scanner++) {
    printf("scanner %d (%d points):\n", idx_scanner, n_points[idx_scanner]);
    for (i32 idx_point = 0; idx_point < n_points[idx_scanner]; idx_point++) {
      printf("  (%f, %f, %f)\n",
        points[idx_scanner][idx_point][0],
        points[idx_scanner][idx_point][1],
        points[idx_scanner][idx_point][2]);
    }
  }
  printf("\n");
}


void print_cob_matrices() {
  for (i32 i = 0; i < 24; i++) {
    glm_mat3_print(cob_matrices[i], stdout);
    f32 det = glm_mat3_det(cob_matrices[i]);
    printf("det: %f\n\n\n", det);
  }
}
#endif
