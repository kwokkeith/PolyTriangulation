/** @file */

#include "DCEL.hpp"
#include <utility>
#include <vector>

/**
 * A function to triangulate monotone partitioned polygon (with the outer face
 * at index 0)
 * @param dcel The DCEL of the monotone partitioned polygon
 * @return A DCEL that contains triangulated polygon
 */
DCEL monotoneTriangulation(DCEL &dcel);

/**
 *  A function to triangulate a face of monotone partitioned polygon (single
 * monotone polygon)
 * @param face Face which is to be triangulated
 * @param dcel DCEL to which face belongs
 * @param diagonals A vector of diagonals to which new diagonal are added.
 */
void faceTriangulation(Face &face, DCEL &dcel,
                       std::vector<std::pair<int, int> > &diagonals);
