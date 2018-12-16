# Fluxions Geometry Transformation Engine (GTE)

The GTE is a template library for doing mathematical operations used throughout the Fluxions library. At some point, it will conform to the TypeScript WebFluxions library as well.

## Classes

| Class Name             | WebFluxions           | TS Implemented |
|------------------------|-----------------------|----------------|
| TMatrix2               | Matrix2               | Yes            |
| TMatrix3               | Matrix3               | Yes            |
| TMatrix4               | Matrix4               | Yes            |
| TMatrix2x3             | Matrix2x3             | No             |
| TMatrix2x4             | Matrix2x4             | No             |
| TMatrix3x2             | Matrix3x2             | No             |
| TMatrix3x4             | Matrix3x4             | No             |
| TMatrix4x2             | Matrix4x2             | No             |
| TMatrix4x4             | Matrix4x3             | No             |
| TVector2               | Vector2               | Yes            |
| TVector3               | Vector3               | Yes            |
| TVector4               | Vector4               | Yes            |
| TBBox                  | BBox                  | Yes            |
| TImage                 | Image                 | Yes            |
| TColor3                | Color3                | Yes            |
| TColor4                | Color4                | Yes            |
| TPlane                 | Plane                 | No             |
| TSuperQuadricEllipsoid | SuperQuadricEllipsoid | No             |
| TSuperQuadricToroid    | SuperQuadricToroid    | No             |
| TRay                   | Ray                   | No             |
| TQuaternion            | Quaternion            | No             |

## Static Functionality

| Operation                  | WebFluxions              | TS Implemented |
|----------------------------|--------------------------|----------------|
| TMatrix4::MakePerspectiveX | Matrix4.makePerspectiveX | Yes            |
