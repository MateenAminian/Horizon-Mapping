#include "World.h"
#include "OpenGEX.h"


using namespace Framework;
using namespace OpenGEX;


WorldManager *Framework::worldManager = nullptr;


Node::Node(uint32 type)
{
	nodeType = type;
	nodeTransform.SetIdentity();
	nodeController = nullptr;
}

Node::~Node()
{
}

void Node::UpdateNode(void)
{
	const Node *superNode = GetSuperNode();
	if (superNode)
	{
		SetWorldTransform(superNode->GetWorldTransform() * nodeTransform);
	}
	else
	{
		SetWorldTransform(nodeTransform);
	}

	Node *node = GetFirstSubnode();
	while (node)
	{
		node->UpdateNode();
		node = node->GetNextSubnode();
	}
}

Node *Node::FindNode(const char *name)
{
	Node *node = GetFirstSubnode();
	while (node)
	{
		if (node->nodeName == name)
		{
			return (node);
		}

		node = GetNextTreeNode(node);
	}

	return (nullptr);
}


CameraNode::CameraNode(uint32 type) : Node(kNodeCamera)
{
	cameraType = type;
}

CameraNode::~CameraNode()
{
}


FrustumCamera::FrustumCamera(float g, float s, float n) : CameraNode(kCameraFrustum)
{
	projectionDistance = g;
	aspectRatio = s;
	nearDepth = n;
}

FrustumCamera::~FrustumCamera()
{
}

Matrix4D FrustumCamera::CalculateProjectionMatrix(void) const
{
	constexpr float kFrustumEpsilon = 1.0F / 524288.0F;

	Matrix4D	matrix;

	matrix(0,1) = matrix(0,2) = matrix(0,3) = matrix(1,0) = matrix(1,2) = matrix(1,3) = matrix(2,0) = matrix(2,1) = matrix(3,0) = matrix(3,1) = matrix(3,3) = 0.0F;
	matrix(3,2) = 1.0F;

	matrix(0,0) = projectionDistance / aspectRatio;
	matrix(1,1) = -projectionDistance;

	matrix(2,2) = (1.0F - kFrustumEpsilon);
	matrix(2,3) = -nearDepth * (1.0F - kFrustumEpsilon);

	return (matrix);
}

void FrustumCamera::UpdateNode(void)
{
	CameraNode::UpdateNode();

	float g = projectionDistance;
	float s = aspectRatio;

	// Construct the camera-space planes.

	float rx = InverseSqrt(g * g + s * s);
	float ry = InverseSqrt(g * g + 1.0F);

	Plane left(g * rx, 0.0F, s * rx, 0.0F);
	Plane right(-g * rx, 0.0F, s * rx, 0.0F);
	Plane top(0.0F, g * ry, ry, 0.0F);
	Plane bottom(0.0F, -g * ry, ry, 0.0F);
	Plane near(0.0F, 0.0F, 1.0F, -nearDepth);

	// Use the inverse of the camera's object-to-world transformation matrix to
	// transform the planes into world space. Planes are row vectors.

	const Transform4D& transform = GetInverseWorldTransform();

	worldFrustumPlane[0] = left * transform;
	worldFrustumPlane[1] = right * transform;
	worldFrustumPlane[2] = top * transform;
	worldFrustumPlane[3] = bottom * transform;
	worldFrustumPlane[4] = near * transform;
}

bool FrustumCamera::SphereVisible(const Point3D& center, float radius) const
{
	// Loop over 5 frustum planes (left, right, top, bottom, near).

	for (int32 k = 0; k < 5; k++)
	{
		const Plane& plane = worldFrustumPlane[k];

		// Check to see if sphere far enough on negative side of frustum plane to cull using Equation (9.11).

		if (Dot(plane, center) < -radius)
		{
			return (false);
		}
	}

	return (true);
}

bool FrustumCamera::BoxVisible(const Transform4D& transform, const Vector3D& size) const
{
	// Calculate the center of the box by adding its half-extents to the position of its minimal corner.

	Vector3D halfAxisX = transform[0] * (size.x * 0.5F);
	Vector3D halfAxisY = transform[1] * (size.y * 0.5F);
	Vector3D halfAxisZ = transform[2] * (size.z * 0.5F);
	Point3D center = transform.GetTranslation() + halfAxisX + halfAxisY + halfAxisZ;

	// Loop over 5 frustum planes (left, right, top, bottom, near).

	for (int32 k = 0; k < 5; k++)
	{
		const Plane& plane = worldFrustumPlane[k];

		// Calculate the effective radius using Equation (9.12).

		float rg = Fabs(Dot(plane, halfAxisX)) + Fabs(Dot(plane, halfAxisY)) + Fabs(Dot(plane, halfAxisZ));

		// Check to see if box far enough on negative side of frustum plane to cull.

		if (Dot(plane, center) <= -rg)
		{
			return (false);
		}
	}

	return (true);
}


OrthoCamera::OrthoCamera(float w, float h) : CameraNode(kCameraOrtho)
{
	orthoWidth = w;
	orthoHeight = h;
}

OrthoCamera::~OrthoCamera()
{
}

Matrix4D OrthoCamera::CalculateProjectionMatrix(void) const
{
	Matrix4D	matrix;

	matrix(0,1) = matrix(0,2) = matrix(1,0) = matrix(1,2) = matrix(2,0) = matrix(2,1) = matrix(2,3) = matrix(3,0) = matrix(3,1) = matrix(3,2) = 0.0F;
	matrix(2,2) = matrix(3,3) = 1.0F;

	matrix(0,0) = 2.0F / orthoWidth;
	matrix(0,3) = -1.0F;

	matrix(1,1) = -2.0F / orthoHeight;
	matrix(1,3) = 1.0F;

	return (matrix);
}


LightNode::LightNode(uint32 type, const ColorRgba& color) : Node(kNodeLight)
{
	lightType = type;
	lightColor = color;
}

LightNode::~LightNode()
{
}

bool LightNode::LightVisible(const FrustumCamera *camera) const
{
	return (true);
}


InfiniteLight::InfiniteLight(const ColorRgba& color) : LightNode(kLightInfinite, color)
{
}

InfiniteLight::~InfiniteLight()
{
}


PointLight::PointLight(const ColorRgba& color, float range) : LightNode(kLightPoint, color)
{
	lightRange = range;
}

PointLight::~PointLight()
{
}

bool PointLight::LightVisible(const FrustumCamera *camera) const
{
	return (camera->SphereVisible(GetWorldPosition(), lightRange));
}

bool PointLight::SphereIlluminated(const Point3D& center, float radius) const
{
	// #LIGHTCULL -- Add code here that determines whether a sphere is illuminated.

	return (true);
}

bool PointLight::BoxIlluminated(const Transform4D& transform, const Vector3D& size) const
{
	// #LIGHTCULL -- Add code here that determines whether a box is illuminated.

	return (true);
}


FogNode::FogNode() : Node(kNodeFog)
{
}

FogNode::~FogNode()
{
}


OccluderNode::OccluderNode(const Vector3D& size) : Node(kNodeOccluder)
{
	occluderSize = size;
}

OccluderNode::~OccluderNode()
{
}

void OccluderNode::CalculateOccluderPlanes(const FrustumCamera *cameraNode)
{
	//# OCCLUDER -- Add code here that calculates the occluder planes for the specific camera position.
	// Store the number of occluder planes in planeCount and the planes themselves in planeArray.

	planeCount = 0;		// If the occluder is not visible (or the camera is inside the occluder), set the plane count to zero.
}

bool OccluderNode::SphereOccluded(const Point3D& center, float radius) const
{
	//# OCCLUDER -- Add code here that determines whether a sphere is occluded.


	return (false);
}

bool OccluderNode::BoxOccluded(const Transform4D& transform, const Vector3D& size) const
{
	//# OCCLUDER -- Add code here that determines whether a box is occluded.


	return (false);
}


BoneNode::BoneNode() : Node(kNodeBone)
{
}

BoneNode::~BoneNode()
{
}


GeometryNode::GeometryNode(uint32 type) : Node(kNodeGeometry), Renderable(Renderable::kTypeIndexedTriangleList)
{
	geometryType = type;
	transformable = this;
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::EstablishStandardVertexArray(void)
{
	vertexArray = new VertexArray;
	vertexArray->SetAttribBuffer(0, sizeof(Vertex), vertexBuffer[0]);
	vertexArray->SetAttribArray(0, 3, VertexArray::kFormatFloat32, 0);
	vertexArray->SetAttribArray(1, 3, VertexArray::kFormatFloat32, sizeof(Point3D));
	vertexArray->SetAttribArray(2, 4, VertexArray::kFormatFloat32, sizeof(Point3D) + sizeof(Bivector3D));
	vertexArray->SetAttribArray(3, 2, VertexArray::kFormatFloat32, sizeof(Point3D) + sizeof(Bivector3D) + sizeof(Vector4D));
}

void GeometryNode::PrepareToRender(const Matrix4D& viewProjectionMatrix)
{
	Matrix4D mvp = viewProjectionMatrix * GetWorldTransform();

	vertexParam[0] = !mvp.row0;
	vertexParam[1] = !mvp.row1;
	vertexParam[2] = !mvp.row2;
	vertexParam[3] = !mvp.row3;

	vertexParam[4] = !GetWorldTransform().row0;
	vertexParam[5] = !GetWorldTransform().row1;
	vertexParam[6] = !GetWorldTransform().row2;

	SetVertexParamCount(7);
}

bool GeometryNode::GeometryVisible(const FrustumCamera *camera) const
{
	return (true);
}

bool GeometryNode::GeometryOccluded(const OccluderNode *occluder) const
{
	return (false);
}

bool GeometryNode::GeometryIlluminated(const PointLight *light) const
{
	return (true);
}


MeshGeometry::MeshGeometry(int32 vertexCount, int32 triangleCount, Vertex *vertexArray, Triangle *triangleArray) : GeometryNode(kGeometryMesh)
{
	meshVertexCount = vertexCount;
	meshTriangleCount = triangleCount;

	meshVertexArray = vertexArray;
	meshTriangleArray = triangleArray;

	SetVertexCount(vertexCount);
	SetIndexCount(triangleCount * 3);

	vertexBuffer[0] = new Buffer(vertexCount * sizeof(Vertex), vertexArray);
	indexBuffer = new Buffer(triangleCount * sizeof(Triangle), triangleArray);

	EstablishStandardVertexArray();
}

MeshGeometry::~MeshGeometry()
{
	delete[] meshTriangleArray;
	delete[] meshVertexArray;
}


SphereGeometry::SphereGeometry(float radius) : GeometryNode(kGeometrySphere)
{
	constexpr float kTexcoordScaleS = 4.0F;
	constexpr float kTexcoordScaleT = 2.0F;

	sphereRadius = radius;

	float dx = 1.0F / float(xdiv);
	float dz = 1.0F / float(zdiv);

	Vertex *vertex = sphereVertex;

	for (machine i = 0; i < xdiv; i++)
	{
		float s = float(i) * dx;
		Vector2D v = CosSin(s * Math::tau);

		for (machine j = 1; j < zdiv; j++)
		{
			float t = float(j) * dz;
			Vector2D u = CosSin((1.0F - t) * Math::tau_over_2);

			float x = v.x * u.y;
			float y = v.y * u.y;
			float z = u.x;

			vertex->position.Set(x * radius, y * radius, z * radius);
			vertex->normal.Set(x, y, z);
			vertex->tangent.Set(-y, x, z, 1.0F);
			vertex->texcoord.Set(s * kTexcoordScaleS, t * kTexcoordScaleT);
			vertex++;
		}

		vertex->position.Set(0.0F, 0.0F, -radius);
		vertex->normal.Set(0.0F, 0.0F, -1.0F);
		vertex->tangent.Set(-v.y, v.x, 0.0F, 1.0F);
		vertex->texcoord.Set(s * kTexcoordScaleS, 0.0F);
		vertex++;

		vertex->position.Set(0.0F, 0.0F, radius);
		vertex->normal.Set(0.0F, 0.0F, 1.0F);
		vertex->tangent.Set(-v.y, v.x, 0.0F, 1.0F);
		vertex->texcoord.Set(s * kTexcoordScaleS, kTexcoordScaleT);
		vertex++;
	}

	for (machine j = 0; j < zdiv - 1; j++)
	{
		vertex->position = sphereVertex[j].position;
		vertex->normal = sphereVertex[j].normal;
		vertex->tangent = sphereVertex[j].tangent;
		vertex->texcoord.Set(kTexcoordScaleS, float(j + 1) * dz * kTexcoordScaleT);
		vertex++;
	}

	Triangle *triangle = sphereTriangle;

	for (machine i = 0; i < xdiv; i++)
	{
		int32 k = i * (zdiv + 1);

		triangle->Set(k + zdiv - 1, k + zdiv + 1, k);
		triangle++;

		for (machine j = 0; j < zdiv - 2; j++)
		{
			if (((i + j) & 1) == 0)
			{
				triangle[0].Set(k + j, k + j + zdiv + 1, k + j + 1);
				triangle[1].Set(k + j + 1, k + j + zdiv + 1, k + j + zdiv + 2);
			}
			else
			{
				triangle[0].Set(k + j, k + j + zdiv + 1, k + j + zdiv + 2);
				triangle[1].Set(k + j, k + j + zdiv + 2, k + j + 1);
			}

			triangle += 2;
		}

		triangle->Set(k + zdiv - 2, k + zdiv * 2 - 1, k + zdiv);
		triangle++;
	}

	// Establish vertex buffers in the Renderable base class.

	SetVertexCount(kSphereVertexCount);
	SetIndexCount(kSphereTriangleCount * 3);

	vertexBuffer[0] = new Buffer(kSphereVertexCount * sizeof(Vertex), sphereVertex);
	indexBuffer = new Buffer(kSphereTriangleCount * sizeof(Triangle), sphereTriangle);

	EstablishStandardVertexArray();
}

SphereGeometry::~SphereGeometry()
{
}

bool SphereGeometry::GeometryVisible(const FrustumCamera *camera) const
{
	return (camera->SphereVisible(GetWorldPosition(), sphereRadius));
}

bool SphereGeometry::GeometryOccluded(const OccluderNode *occluder) const
{
	return (occluder->SphereOccluded(GetWorldPosition(), sphereRadius));
}

bool SphereGeometry::GeometryIlluminated(const PointLight *light) const
{
	return (light->SphereIlluminated(GetWorldPosition(), sphereRadius));
}


BoxGeometry::BoxGeometry(const Vector3D& size) : GeometryNode(kGeometryBox)
{
	boxSize = size;

	// Positive x side

	boxVertex[0].position.Set(size.x, 0.0F, 0.0F);
	boxVertex[1].position.Set(size.x, size.y, 0.0F);
	boxVertex[2].position.Set(size.x, size.y, size.z);
	boxVertex[3].position.Set(size.x, 0.0F, size.z);

	boxVertex[0].normal.Set(1.0F, 0.0F, 0.0F);
	boxVertex[1].normal.Set(1.0F, 0.0F, 0.0F);
	boxVertex[2].normal.Set(1.0F, 0.0F, 0.0F);
	boxVertex[3].normal.Set(1.0F, 0.0F, 0.0F);

	boxVertex[0].tangent.Set(0.0F, 1.0F, 0.0F, 1.0F);
	boxVertex[1].tangent.Set(0.0F, 1.0F, 0.0F, 1.0F);
	boxVertex[2].tangent.Set(0.0F, 1.0F, 0.0F, 1.0F);
	boxVertex[3].tangent.Set(0.0F, 1.0F, 0.0F, 1.0F);

	boxVertex[0].texcoord.Set(0.0F, 0.0F);
	boxVertex[1].texcoord.Set(size.y, 0.0F);
	boxVertex[2].texcoord.Set(size.y, size.z);
	boxVertex[3].texcoord.Set(0.0F, size.z);

	boxTriangle[0].index[0] = 0;
	boxTriangle[0].index[1] = 1;
	boxTriangle[0].index[2] = 2;

	boxTriangle[1].index[0] = 0;
	boxTriangle[1].index[1] = 2;
	boxTriangle[1].index[2] = 3;

	// Positive y side

	boxVertex[4].position.Set(size.x, size.y, 0.0F);
	boxVertex[5].position.Set(0.0F, size.y, 0.0F);
	boxVertex[6].position.Set(0.0F, size.y, size.z);
	boxVertex[7].position.Set(size.x, size.y, size.z);

	boxVertex[4].normal.Set(0.0F, 1.0F, 0.0F);
	boxVertex[5].normal.Set(0.0F, 1.0F, 0.0F);
	boxVertex[6].normal.Set(0.0F, 1.0F, 0.0F);
	boxVertex[7].normal.Set(0.0F, 1.0F, 0.0F);

	boxVertex[4].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[5].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[6].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[7].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);

	boxVertex[4].texcoord.Set(0.0F, 0.0F);
	boxVertex[5].texcoord.Set(size.x, 0.0F);
	boxVertex[6].texcoord.Set(size.x, size.z);
	boxVertex[7].texcoord.Set(0.0F, size.z);

	boxTriangle[2].index[0] = 4;
	boxTriangle[2].index[1] = 5;
	boxTriangle[2].index[2] = 6;

	boxTriangle[3].index[0] = 4;
	boxTriangle[3].index[1] = 6;
	boxTriangle[3].index[2] = 7;

	// Negative x side

	boxVertex[8].position.Set(0.0F, size.y, 0.0F);
	boxVertex[9].position.Set(0.0F, 0.0F, 0.0F);
	boxVertex[10].position.Set(0.0F, 0.0F, size.z);
	boxVertex[11].position.Set(0.0F, size.y, size.z);

	boxVertex[8].normal.Set(-1.0F, 0.0F, 0.0F);
	boxVertex[9].normal.Set(-1.0F, 0.0F, 0.0F);
	boxVertex[10].normal.Set(-1.0F, 0.0F, 0.0F);
	boxVertex[11].normal.Set(-1.0F, 0.0F, 0.0F);

	boxVertex[8].tangent.Set(0.0F, -1.0F, 0.0F, 1.0F);
	boxVertex[9].tangent.Set(0.0F, -1.0F, 0.0F, 1.0F);
	boxVertex[10].tangent.Set(0.0F, -1.0F, 0.0F, 1.0F);
	boxVertex[11].tangent.Set(0.0F, -1.0F, 0.0F, 1.0F);

	boxVertex[8].texcoord.Set(0.0F, 0.0F);
	boxVertex[9].texcoord.Set(size.y, 0.0F);
	boxVertex[10].texcoord.Set(size.y, size.z);
	boxVertex[11].texcoord.Set(0.0F, size.z);

	boxTriangle[4].index[0] = 8;
	boxTriangle[4].index[1] = 9;
	boxTriangle[4].index[2] = 10;

	boxTriangle[5].index[0] = 8;
	boxTriangle[5].index[1] = 10;
	boxTriangle[5].index[2] = 11;

	// Negative y side

	boxVertex[12].position.Set(0.0F, 0.0F, 0.0F);
	boxVertex[13].position.Set(size.x, 0.0F, 0.0F);
	boxVertex[14].position.Set(size.x, 0.0F, size.z);
	boxVertex[15].position.Set(0.0F, 0.0F, size.z);

	boxVertex[12].normal.Set(0.0F, -1.0F, 0.0F);
	boxVertex[13].normal.Set(0.0F, -1.0F, 0.0F);
	boxVertex[14].normal.Set(0.0F, -1.0F, 0.0F);
	boxVertex[15].normal.Set(0.0F, -1.0F, 0.0F);

	boxVertex[12].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[13].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[14].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[15].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);

	boxVertex[12].texcoord.Set(0.0F, 0.0F);
	boxVertex[13].texcoord.Set(size.x, 0.0F);
	boxVertex[14].texcoord.Set(size.x, size.z);
	boxVertex[15].texcoord.Set(0.0F, size.z);

	boxTriangle[6].index[0] = 12;
	boxTriangle[6].index[1] = 13;
	boxTriangle[6].index[2] = 14;

	boxTriangle[7].index[0] = 12;
	boxTriangle[7].index[1] = 14;
	boxTriangle[7].index[2] = 15;

	// Positive z side

	boxVertex[16].position.Set(0.0F, 0.0F, size.z);
	boxVertex[17].position.Set(size.x, 0.0F, size.z);
	boxVertex[18].position.Set(size.x, size.y, size.z);
	boxVertex[19].position.Set(0.0F, size.y, size.z);

	boxVertex[16].normal.Set(0.0F, 0.0F, 1.0F);
	boxVertex[17].normal.Set(0.0F, 0.0F, 1.0F);
	boxVertex[18].normal.Set(0.0F, 0.0F, 1.0F);
	boxVertex[19].normal.Set(0.0F, 0.0F, 1.0F);

	boxVertex[16].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[17].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[18].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[19].tangent.Set(1.0F, 0.0F, 0.0F, 1.0F);

	boxVertex[16].texcoord.Set(0.0F, 0.0F);
	boxVertex[17].texcoord.Set(size.x, 0.0F);
	boxVertex[18].texcoord.Set(size.x, size.y);
	boxVertex[19].texcoord.Set(0.0F, size.y);

	boxTriangle[8].index[0] = 16;
	boxTriangle[8].index[1] = 17;
	boxTriangle[8].index[2] = 18;

	boxTriangle[9].index[0] = 16;
	boxTriangle[9].index[1] = 18;
	boxTriangle[9].index[2] = 19;

	// Negative z side

	boxVertex[20].position.Set(size.x, 0.0F, 0.0F);
	boxVertex[21].position.Set(0.0F, 0.0F, 0.0F);
	boxVertex[22].position.Set(0.0F, size.y, 0.0F);
	boxVertex[23].position.Set(size.x, size.y, 0.0F);

	boxVertex[20].normal.Set(0.0F, 0.0F, -1.0F);
	boxVertex[21].normal.Set(0.0F, 0.0F, -1.0F);
	boxVertex[22].normal.Set(0.0F, 0.0F, -1.0F);
	boxVertex[23].normal.Set(0.0F, 0.0F, -1.0F);

	boxVertex[20].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[21].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[22].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);
	boxVertex[23].tangent.Set(-1.0F, 0.0F, 0.0F, 1.0F);

	boxVertex[20].texcoord.Set(0.0F, 0.0F);
	boxVertex[21].texcoord.Set(size.x, 0.0F);
	boxVertex[22].texcoord.Set(size.x, size.y);
	boxVertex[23].texcoord.Set(0.0F, size.y);

	boxTriangle[10].index[0] = 20;
	boxTriangle[10].index[1] = 21;
	boxTriangle[10].index[2] = 22;

	boxTriangle[11].index[0] = 20;
	boxTriangle[11].index[1] = 22;
	boxTriangle[11].index[2] = 23;

	// Establish vertex buffers in the Renderable base class.

	SetVertexCount(24);
	SetIndexCount(36);

	vertexBuffer[0] = new Buffer(24 * sizeof(Vertex), boxVertex);
	indexBuffer = new Buffer(12 * sizeof(Triangle), boxTriangle);

	EstablishStandardVertexArray();
}

BoxGeometry::~BoxGeometry()
{
}

bool BoxGeometry::GeometryVisible(const FrustumCamera *camera) const
{
	return (camera->BoxVisible(GetWorldTransform(), boxSize));
}

bool BoxGeometry::GeometryOccluded(const OccluderNode *occluder) const
{
	return (occluder->BoxOccluded(GetWorldTransform(), boxSize));
}

bool BoxGeometry::GeometryIlluminated(const PointLight *light) const
{
	return (light->BoxIlluminated(GetWorldTransform(), boxSize));
}


TextGeometry::TextGeometry(Font *font, float size, const char *text, const Color4U& color) : GeometryNode(kGeometryText)
{
	static const char vertexMain[] =
	{
		"layout(location = 0) in float4 attrib0;\n"
		"layout(location = 1) in float4 attrib1;\n"
		"layout(location = 2) in float4 attrib2;\n"
		"layout(location = 3) in float4 attrib3;\n"
		"layout(location = 4) in float4 attrib4;\n"

		"layout(location = 0) uniform float4 slug_matrix[5];\n"

		"out float4 vresult_color;\n"
		"out float2 vresult_texcoord;\n"
		"flat out float4 vresult_banding;\n"
		"flat out int4 vresult_glyph;\n"

		"void main()\n"
		"{\n"
			"float2 p;\n"

			"vresult_texcoord = SlugDilate(attrib0, attrib1, attrib2, slug_matrix[0], slug_matrix[1], slug_matrix[3], slug_matrix[4].xy, p);\n"
			"gl_Position.x = p.x * slug_matrix[0].x + p.y * slug_matrix[0].y + slug_matrix[0].w;\n"
			"gl_Position.y = p.x * slug_matrix[1].x + p.y * slug_matrix[1].y + slug_matrix[1].w;\n"
			"gl_Position.z = p.x * slug_matrix[2].x + p.y * slug_matrix[2].y + slug_matrix[2].w;\n"
			"gl_Position.w = p.x * slug_matrix[3].x + p.y * slug_matrix[3].y + slug_matrix[3].w;\n"

			"SlugUnpack(attrib1, attrib3, vresult_banding, vresult_glyph);\n"
			"vresult_color = attrib4;\n"
		"}\n"
	};

	static const char fragmentMain[] =
	{
		"in vec4 vresult_color;\n"
		"in vec2 vresult_texcoord;\n"
		"flat in vec4 vresult_banding;\n"
		"flat in ivec4 vresult_glyph;\n"

		"layout(binding = 0) uniform sampler2D curveTexture;\n"
		"layout(binding = 1) uniform usampler2D bandTexture;\n"

		"layout(location = 0) out float4 fcolor;\n"

		"void main()\n"
		"{\n"
			"fcolor = SlugRender(curveTexture, bandTexture, vresult_texcoord, vresult_color, vresult_banding, vresult_glyph);\n"
		"}\n"
	};

	uint32			vertexIndex;
	uint32			fragmentIndex;
	const char		*vertexStringArray[Slug::kMaxVertexStringCount + 1];
	const char		*fragmentStringArray[Slug::kMaxFragmentStringCount + 1];

	textFont = font;
	font->Retain();

	textColor = color;
	fontSize = size;

	if (text)
	{
		SetText(text);
	}

	SetBlendMode(Renderable::kBlendAlpha);
	SetDepthWriteFlag(false);
	SetCullFaceFlag(false);

	SetTextureCount(2);
	SetTexture(0, font->GetCurveTexture());
	SetTexture(1, font->GetBandTexture());

	Slug::GetShaderIndices(0, &vertexIndex, &fragmentIndex);

	vertexStringArray[0] = fragmentStringArray[0] = "#version 450\n";
	int32 vertexStringCount = Slug::GetVertexShaderSourceCode(vertexIndex, &vertexStringArray[1], Slug::kVertexShaderProlog);
	int32 fragmentStringCount = Slug::GetFragmentShaderSourceCode(fragmentIndex, &fragmentStringArray[1], Slug::kFragmentShaderProlog);
	vertexStringArray[vertexStringCount + 1] = vertexMain;
	fragmentStringArray[fragmentStringCount + 1] = fragmentMain;

	Program *program = new Program(vertexStringCount + 2, vertexStringArray, fragmentStringCount + 2, fragmentStringArray);
	SetProgram(0, program);
	program->Release();
}

TextGeometry::~TextGeometry()
{
}

void TextGeometry::SetText(const char *text)
{
	Slug::LayoutData		layoutData;
	Slug::GeometryBuffer	geometryBuffer;
	int32					maxVertexCount;
	int32					maxTriangleCount;

	delete indexBuffer;
	delete vertexBuffer[0];

	Slug::SetDefaultLayoutData(&layoutData);
	layoutData.fontSize = fontSize;
	layoutData.textColor.color[0] = textColor;

	Slug::CountSlug(textFont->GetFontHeader(), &layoutData, text, -1, &maxVertexCount, &maxTriangleCount);

	vertexBuffer[0] = new Buffer(maxVertexCount * sizeof(Slug::Vertex));
	indexBuffer = new Buffer(maxTriangleCount * sizeof(Slug::Triangle));

	geometryBuffer.vertexData = static_cast<volatile Slug::Vertex *>(vertexBuffer[0]->MapBuffer());
	geometryBuffer.triangleData = static_cast<volatile Slug::Triangle *>(indexBuffer->MapBuffer());
	geometryBuffer.vertexIndex = 0;

	volatile Slug::Vertex *vertexBase = geometryBuffer.vertexData;
	volatile Slug::Triangle *triangleBase = geometryBuffer.triangleData;

	Slug::BuildSlug(textFont->GetFontHeader(), &layoutData, text, -1, Point2D(0.0F, 0.0F), &geometryBuffer);

	indexBuffer->UnmapBuffer();
	vertexBuffer[0]->UnmapBuffer();

	SetVertexCount(int32(geometryBuffer.vertexData - vertexBase));
	SetIndexCount(int32(geometryBuffer.triangleData - triangleBase) * 3);

	vertexArray = new VertexArray;
	vertexArray->SetAttribBuffer(0, sizeof(Slug::Vertex), vertexBuffer[0]);
	vertexArray->SetAttribArray(0, 4, VertexArray::kFormatFloat32, 0);
	vertexArray->SetAttribArray(1, 4, VertexArray::kFormatFloat32, sizeof(Vector4D));
	vertexArray->SetAttribArray(2, 4, VertexArray::kFormatFloat32, sizeof(Vector4D) * 2);
	vertexArray->SetAttribArray(3, 4, VertexArray::kFormatFloat32, sizeof(Vector4D) * 3);
	vertexArray->SetAttribArray(4, 4, VertexArray::kFormatUint8, sizeof(Vector4D) * 4);
}

void TextGeometry::PrepareToRender(const Matrix4D& viewProjectionMatrix)
{
	GeometryNode::PrepareToRender(viewProjectionMatrix);

	vertexParam[4].Set(graphicsManager->GetViewportWidth(), graphicsManager->GetViewportHeight(), 0.0F, 0.0F);
	SetVertexParamCount(5);
}


Controller::Controller(Node *node)
{
	targetNode = node;
}

Controller::~Controller()
{
}

void Controller::PreprocessController(void)
{
}

void Controller::MoveController(void)
{
}

void Controller::UpdateController(void)
{
}


LightController::LightController(LightNode *node) : Controller(node)
{
	revolutionAxis = RandomUnitBivector3D();
}

LightController::~LightController()
{
}

void LightController::MoveController(void)
{
	extern float deltaTime;

	float angle = deltaTime * 0.2F;
	targetNode->nodeTransform[3] = Matrix3D::MakeRotation(angle, revolutionAxis) * targetNode->nodeTransform[3];
}


SkinController::SkinController(MeshGeometry *mesh) : Controller(mesh)
{
	updateFlag = true;

	skinDataStorage = nullptr;
	transformTable = nullptr;
}

SkinController::~SkinController()
{
	delete[] transformTable;
	delete[] skinDataStorage;
}

void SkinController::PreprocessController(void)
{
	transformTable = new Transform4D[boneNodeArray.GetArrayElementCount()];
}

void SkinController::UpdateController(void)
{
	const MeshGeometry *meshGeometry = GetTargetNode();
	//int32 totalBoneCount = boneNodeArray.GetArrayElementCount();

	// #SKIN -- Add code here to calculate a table containing the matrix product
	//
	// (mesh inverse world transform) * (current bone world transform) * (bone inverse bind transform)
	//
	// for each bone. The array boneNodeArray contains pointers to the bone nodes, and inverseBindTransformArray contains the
	// inverse bind transforms for the same bones. Store the results in the transformTable array, which has already been
	// allocated with enough space for all of the bones.

	int32 vertexCount = meshGeometry->meshVertexCount;
	const Vertex *bindVertex = meshGeometry->meshVertexArray;

	//const SkinData *skinData = GetSkinData();
	volatile Vertex *skinnedVertex = static_cast<volatile Vertex *>(meshGeometry->vertexBuffer[0]->MapBuffer());

	for (int32 i = 0; i < vertexCount; i++)
	{
		// #SKIN -- Add code here to calculate skinned vertex positions, normals, and tangents.
		// The bind positions, normals, and tangents are stored in the bindVertex array.
		// Store the results in the skinnedVertexArray, but only once after calculating the
		// final positions, normals, and tangents. Copy the texcoords without modification.
		//
		// The skinData storage contains all of the bone influence data for all vertices.
		//
		// You may use the ! operator to convert Vector3D <-> Bivector3D.
		//
		// (Replace the following code that simply copies the bind data.)

		skinnedVertex[i].position = bindVertex[i].position;
		skinnedVertex[i].normal = bindVertex[i].normal;
		skinnedVertex[i].tangent = bindVertex[i].tangent;
		skinnedVertex[i].texcoord = bindVertex[i].texcoord;
	}

	meshGeometry->vertexBuffer[0]->UnmapBuffer();
}


WorldManager::WorldManager()
{
	rootNode = new Node(0);

	cameraNode = nullptr;
	fogNode = nullptr;

	ambientColor.Set(0.0F, 0.0F, 0.0F, 0.0F);

	overlayCameraNode = nullptr;
}

WorldManager::~WorldManager()
{
	geometryList.RemoveAllListElements();
	occluderList.RemoveAllListElements();
	lightList.RemoveAllListElements();

	delete rootNode;
	delete overlayCameraNode;
}

void WorldManager::PreprocessWorld(void)
{
	Node *node = rootNode->GetFirstSubnode();
	while (node)
	{
		uint32 type = node->GetNodeType();
		if (type == kNodeGeometry)
		{
			geometryList.AppendListElement(static_cast<GeometryNode *>(node));
		}
		else if (type == kNodeLight)
		{
			lightList.AppendListElement(static_cast<LightNode *>(node));
		}
		else if (type == kNodeCamera)
		{
			if (!cameraNode)
			{
				CameraNode *camera = static_cast<CameraNode *>(node);
				if (camera->GetCameraType() == kCameraFrustum)
				{
					cameraNode = static_cast<FrustumCamera *>(camera);
				}
			}
		}
		else if (type == kNodeFog)
		{
			FogNode *fog = static_cast<FogNode *>(node);
			fogNode = fog;
		}
		else if (type == kNodeOccluder)
		{
			occluderList.AppendListElement(static_cast<OccluderNode *>(node));
		}

		Controller *controller = node->nodeController;
		if (controller)
		{
			controllerList.AppendListElement(controller);
			controller->PreprocessController();
		}

		node = rootNode->GetNextTreeNode(node);
	}

	for (GeometryNode *geometryNode : overlayGeometryList)
	{
		geometryNode->SetWorldTransform(geometryNode->nodeTransform);
	}
}

void WorldManager::RenderWorld(int32 *ambientDrawCount, int32 *lightDrawCount, int32 *lightSourceCount)
{
	Array<GeometryNode *>		visibleGeometryArray;

	int32 pointLightCount = 0;
	int32 illuminatedCount = 0;

	if (cameraNode)
	{
		for (Controller *controller : controllerList)
		{
			controller->MoveController();
		}

		rootNode->UpdateNode();

		for (Controller *controller : controllerList)
		{
			if (controller->updateFlag)
			{
				controller->updateFlag = false;
				controller->UpdateController();
			}
		}

		UniversalParams *params = graphicsManager->GetUniversalParams();
		params->cameraPosition = cameraNode->GetWorldPosition();
		params->cameraRight = cameraNode->GetWorldTransform()[0];
		params->cameraDown = cameraNode->GetWorldTransform()[1];
		params->ambientColor = ambientColor;

		if (fogNode)
		{
			// #FOG -- Add code here to establish the fog plane, color, and other parameters in the UniversalParams structure.


		}

		for (OccluderNode *occluderNode : occluderList)
		{
			occluderNode->CalculateOccluderPlanes(cameraNode);
		}

		Matrix4D viewProjectionMatrix = cameraNode->CalculateProjectionMatrix() * cameraNode->GetInverseWorldTransform();

		// Render ambient pass.

		for (GeometryNode *geometryNode : geometryList)
		{
			if (geometryNode->GeometryVisible(cameraNode))
			{
				visibleGeometryArray.AppendArrayElement(geometryNode);

				geometryNode->PrepareToRender(viewProjectionMatrix);
				geometryNode->Render(0);
			}
		}

		// Render light passes.

		for (const LightNode *lightNode : lightList)
		{
			if (lightNode->GetLightType() == kLightPoint)
			{
				const PointLight *pointLight = static_cast<const PointLight *>(lightNode);

				if (lightNode->LightVisible(cameraNode))
				{
					params = graphicsManager->GetUniversalParams();
					params->lightColor = lightNode->lightColor;
					params->lightPosition = lightNode->GetWorldPosition();

					float r = pointLight->lightRange;
					params->attenConst.Set(-4.0F / (r * r), 1.01865736F, 0.01865736F, 0.0F);

					for (GeometryNode *geometryNode : visibleGeometryArray)
					{
						if (geometryNode->GeometryIlluminated(pointLight))
						{
							geometryNode->PrepareToRender(viewProjectionMatrix);
							geometryNode->Render(1);
							illuminatedCount++;
						}
					}

					pointLightCount++;
				}
			}
		}
	}

	*ambientDrawCount = visibleGeometryArray.GetArrayElementCount();
	*lightDrawCount = illuminatedCount;
	*lightSourceCount = pointLightCount;
}

void WorldManager::RenderOverlay(void)
{
	if (overlayCameraNode)
	{
		Matrix4D projectionMatrix = overlayCameraNode->CalculateProjectionMatrix();

		for (GeometryNode *geometryNode : overlayGeometryList)
		{
			geometryNode->PrepareToRender(projectionMatrix);
			geometryNode->Render(0);
		}
	}
}

void WorldManager::ConstructNormalMap(const Color4U *heightMap, Color2S *normalMap, int32 width, int32 height, float scale)
{
	for (int32 y = 0; y < height; y++)
	{
		int32 ym1 = (y - 1) & (height - 1);
		int32 yp1 = (y + 1) & (height - 1);

		const Color4U *centerRow = heightMap + y * width;
		const Color4U *upperRow = heightMap + ym1 * width;
		const Color4U *lowerRow = heightMap + yp1 * width;

		for (int32 x = 0; x < width; x++)
		{
			int32 xm1 = (x - 1) & (width - 1);
			int32 xp1 = (x + 1) & (width - 1);

			// Calculate slopes.
			float dx = float(centerRow[xp1].red - centerRow[xm1].red) * (0.5F / 255.0F) * scale;
			float dy = float(lowerRow[x].red - upperRow[x].red) * (0.5F / 255.0F) * scale;

			// Normalize and clamp.
			float nz = 1.0F / Sqrt(dx * dx + dy * dy + 1.0F);
			float nx = Clamp(-dx * nz, -1.0F, 1.0F);
			float ny = Clamp(-dy * nz, -1.0F, 1.0F);

			normalMap[x].red = int8(nx * 127.0F);
			normalMap[x].green = int8(ny * 127.0F);
		}

		normalMap += width;
	}
}

void WorldManager::ConstructParallaxMap(const Color4U *heightMap, Color1S *parallaxMap, int32 width, int32 height, float scale)
{
	// #PARALLAX -- Add code here to construct a single-channel parallax map.


}

void WorldManager::ConstructHorizonMap(const Color4U *heightMap, Color4U *horizonMap, int32 width, int32 height, float scale)
{
	// #HORIZON -- Add code here to construct an eight-channel horizon map.
	// Construct the two layers as two four-channel images stored one after the other.
	// LISTING  7.10 - main
	constexpr int kAngleCount = 32; // Must be at least 16 and a power of 2.
	constexpr float kAngleIndex = float(kAngleCount) / float(2*3.14519);
	constexpr int kHorizonRadius = 16;
	for (int32 y = 0; y < height; y++)
	{
		const Color4U* centerRow = heightMap + y * width;
		for (int32 x = 0; x < width; x++)
		{
			// Get central height. Initialize max squared tangent array to all zeros.
			float h0 = centerRow[x].red;
			float maxTan2[kAngleCount] = {};
			// Search neighborhood for larger heights.
			for (int32 j = -kHorizonRadius + 1; j < kHorizonRadius; j++)
			{
				const Color4U* row = heightMap + ((y + j) & (height - 1)) * width;
				for (int32 i = -kHorizonRadius + 1; i < kHorizonRadius; i++)
				{
					int32 r2 = i * i + j * j;
					if ((r2 < kHorizonRadius * kHorizonRadius) && (r2 != 0))
					{
						float dh = row[(x + i) & (width - 1)].red - h0;
						if (dh > 0.0F)
						{
							// Larger height found. Apply to array entries.
							float direction = atan2(float(j), float(i));
							float delta = atan(0.7071F / sqrt(float(r2)));
							int32 minIndex = int32(floor((direction - delta) * kAngleIndex));
							int32 maxIndex = int32(ceil((direction + delta) * kAngleIndex));
							// Calculate squared tangent with Equation (7.53). 
							float t = dh * dh / float(r2);
							for (int32 n = minIndex; n <= maxIndex; n++)
							{
								int32 m = n & (kAngleCount - 1);
								maxTan2[m] = fmax(maxTan2[m], t);
							}
						}
					}
				}
			}
		
			// Generate eight channels of horizon map.
			Color4U* layerData = horizonMap;
			for (int32 layer = 0; layer < 2; layer++)
			{
				ColorRgba color(0.0F, 0.0F, 0.0F, 0.0F);
				int32 firstIndex = kAngleCount / 16 + layer * (kAngleCount / 2);
				int32 lastIndex = firstIndex + kAngleCount / 8;
				for (int32 index = firstIndex; index <= lastIndex; index++)
				{
					float tr = maxTan2[(index - kAngleCount / 8) & (kAngleCount - 1)];
					float tg = maxTan2[index];
					float tb = maxTan2[index + kAngleCount / 8];
					float ta = maxTan2[(index + kAngleCount / 4) & (kAngleCount - 1)];
					color.red += sqrt(tr / (tr + 1.0F));
					color.green += sqrt(tg / (tg + 1.0F));
					color.blue += sqrt(tb / (tb + 1.0F));
					color.alpha += sqrt(ta / (ta + 1.0F));
				}
				layerData[x] = color / float(kAngleCount / 8 + 1);
				layerData += width * height;
			}
			// Generate ambient light factor.
			float sum = 0.0F;
			for (int32 k = 0; k < kAngleCount; k++) sum += 1.0F / sqrt(maxTan2[k] + 1.0F);
			//ambientMap[x] = pow(sum * float(kAngleCount), ambientPower);
		}
		horizonMap += width;
		//ambientMap += width;
	}
	// listing 7.12 (implement horizon mapping)


}

void WorldManager::GenerateHorizonCube(Color4U* texel) {
	for (int face = 0; face < 6; face++)
	{
		for (float y = -0.9375F; y < 1.0F; y += 0.125F)
		{
			for (float x = -0.9375F; x < 1.0F; x += 0.125F)
			{
				Vector2D v;
				float r = 1.0F / sqrt(1.0F + x * x + y * y);
				switch (face)
				{
				case 0: v.Set(r, -y * r); break;
				case 1: v.Set(-r, -y * r); break;
				case 2: v.Set(x * r, r); break;
				case 3: v.Set(x * r, -r); break;
				case 4: v.Set(x * r, -y * r); break;
				case 5: v.Set(-x * r, -y * r); break;
				}
				float t = atan2(v.y, v.x) / (3.14519/4);
				float red = 0.0F;
				float green = 0.0F;
				float blue = 0.0F;
				float alpha = 0.0F;
				if (t < -3.0F) { red = t + 3.0F; green = -4.0F - t; }
				else if (t < -2.0F) { green = t + 2.0F; blue = -3.0F - t; }
				else if (t < -1.0F) { blue = t + 1.0F; alpha = -2.0F - t; }
				else if (t < 0.0F) { alpha = t; red = t + 1.0F; }
				else if (t < 1.0F) { red = 1.0F - t; green = t; }
				else if (t < 2.0F) { green = 2.0F - t; blue = t - 1.0F; }
				else if (t < 3.0F) { blue = 3.0F - t; alpha = t - 2.0F; }
				else { alpha = 4.0F - t; red = 3.0F - t; }
				texel->Set(red, green, blue, alpha);
				texel++;
			}
		}
	}
}

void WorldManager::BuildFloor(Program *ambientProgram, Program *lightProgram)
{
	Color4U			*textureImage;
	Color4U			*colorMipmapImages;
	Color4U         *horizonMipmapImages;
	Color2S			*normalMipmapImages;
	Integer2D		imageSize;

	// Diffuse texture

	ImportTargaImageFile("Textures/StoneFloor-diff.tga", &textureImage, &imageSize);
	int32 mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *floorDiffuseTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Normal texture

	ImportTargaImageFile("Textures/StoneFloor-nrml.tga", &textureImage, &imageSize);
	Color2S *normalMap = new Color2S[imageSize.x * imageSize.y];
	ConstructNormalMap(textureImage, normalMap, imageSize.x, imageSize.y, 16.0F);

	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), normalMap, &normalMipmapImages);
	delete[] normalMap;

	Texture *floorNormalTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, normalMipmapImages);

	ReleaseMipmapImages(normalMipmapImages);
	ReleaseTargaImageData(textureImage);

	// horizon texture
	//ConstructHorizonMap(heightMap, horizonMap, imageSize.x, imageSize.y, 1);
	//mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &horizonMipmapImages);
	//Texture *floorHorizonTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, horizonMipmapImages);
	
	//ReleaseMipmapImages(horizonMipmapImages);
	//ReleaseTargaImageData(textureImage);

	// Geometry

	BoxGeometry *boxGeometry = new BoxGeometry(Vector3D(100.0F, 100.0F, 1.0F));
	boxGeometry->nodeTransform.SetTranslation(Point3D(-50.0F, -50.0F, -1.0F));
	rootNode->AppendSubnode(boxGeometry);

	boxGeometry->SetTexture(0, floorDiffuseTexture);
	boxGeometry->SetTexture(1, floorNormalTexture);
	//boxGeometry->SetTexture(2, horizonTexture);
	//boxGeometry->SetTexture(3, horizonCubeTexture);
	boxGeometry->SetTextureCount(2);

	boxGeometry->SetProgram(0, ambientProgram);
	boxGeometry->SetProgram(1, lightProgram);

	boxGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	boxGeometry->fragmentParam[1].Set(0.2F, 0.2F, 0.2F, 150.0F);
	boxGeometry->SetFragmentParamLocation(32);
	boxGeometry->SetFragmentParamCount(2);

	floorNormalTexture->Release();
	floorDiffuseTexture->Release();
}

void WorldManager::BuildWalls(Program *ambientProgram, Program *lightProgram, Texture *diffuseTexture, Texture *normalTexture)
{
	// Negative y

	BoxGeometry *boxGeometry = new BoxGeometry(Vector3D(100.0F, 1.0F, 20.0F));
	boxGeometry->nodeTransform.SetTranslation(Point3D(-50.0F, -50.0F, 0.0F));
	rootNode->AppendSubnode(boxGeometry);

	boxGeometry->SetTexture(0, diffuseTexture);
	boxGeometry->SetTexture(1, normalTexture);
	//boxGeometry->SetTexture(2, horizonTexture);
	//boxGeometry->SetTexture(3, horizonCubeTexture);
	boxGeometry->SetTextureCount(2);

	boxGeometry->SetProgram(0, ambientProgram);
	boxGeometry->SetProgram(1, lightProgram);

	boxGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	boxGeometry->fragmentParam[1].Set(0.05F, 0.05F, 0.05F, 150.0F);
	boxGeometry->SetFragmentParamLocation(32);
	boxGeometry->SetFragmentParamCount(2);

	// Positive y

	boxGeometry = new BoxGeometry(Vector3D(100.0F, 1.0F, 20.0F));
	boxGeometry->nodeTransform.SetTranslation(Point3D(-50.0F, 49.0F, 0.0F));
	rootNode->AppendSubnode(boxGeometry);

	boxGeometry->SetTexture(0, diffuseTexture);
	boxGeometry->SetTexture(1, normalTexture);
	//boxGeometry->SetTexture(2, horizonTexture);
	//boxGeometry->SetTexture(3, horizonCubeTexture);
	boxGeometry->SetTextureCount(2);

	boxGeometry->SetProgram(0, ambientProgram);
	boxGeometry->SetProgram(1, lightProgram);

	boxGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	boxGeometry->fragmentParam[1].Set(0.05F, 0.05F, 0.05F, 150.0F);
	boxGeometry->SetFragmentParamLocation(32);
	boxGeometry->SetFragmentParamCount(2);

	// Negative x

	boxGeometry = new BoxGeometry(Vector3D(1.0F, 98.0F, 20.0F));
	boxGeometry->nodeTransform.SetTranslation(Point3D(-50.0F, -49.0F, 0.0F));
	rootNode->AppendSubnode(boxGeometry);

	boxGeometry->SetTexture(0, diffuseTexture);
	boxGeometry->SetTexture(1, normalTexture);
	//boxGeometry->SetTexture(2, horizonTexture);
	//boxGeometry->SetTexture(3, horizonCubeTexture);
	boxGeometry->SetTextureCount(2);

	boxGeometry->SetProgram(0, ambientProgram);
	boxGeometry->SetProgram(1, lightProgram);

	boxGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	boxGeometry->fragmentParam[1].Set(0.05F, 0.05F, 0.05F, 150.0F);
	boxGeometry->SetFragmentParamLocation(32);
	boxGeometry->SetFragmentParamCount(2);

	// Positive x

	boxGeometry = new BoxGeometry(Vector3D(1.0F, 98.0F, 20.0F));
	boxGeometry->nodeTransform.SetTranslation(Point3D(49.0F, -49.0F, 0.0F));
	rootNode->AppendSubnode(boxGeometry);

	boxGeometry->SetTexture(0, diffuseTexture);
	boxGeometry->SetTexture(1, normalTexture);
	//boxGeometry->SetTexture(2, horizonTexture);
	//boxGeometry->SetTexture(3, horizonCubeTexture);
	boxGeometry->SetTextureCount(2);

	boxGeometry->SetProgram(0, ambientProgram);
	boxGeometry->SetProgram(1, lightProgram);

	boxGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	boxGeometry->fragmentParam[1].Set(0.05F, 0.05F, 0.05F, 150.0F);
	boxGeometry->SetFragmentParamLocation(32);
	boxGeometry->SetFragmentParamCount(2);
}

void WorldManager::BuildTree(const Point3D& position)
{
	Array<MeshGeometry *>	meshArray;
	Color4U					*textureImage;
	Color4U					*colorMipmapImages;
	Color2S					*normalMipmapImages;
	Integer2D				imageSize;

	// Trunk diffuse texture

	ImportTargaImageFile("Textures/Trunk-diff.tga", &textureImage, &imageSize);
	int32 mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *trunkDiffuseTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Trunk normal texture

	ImportTargaImageFile("Textures/Trunk-nrml.tga", &textureImage, &imageSize);
	Color2S *normalMap = new Color2S[imageSize.x * imageSize.y];
	ConstructNormalMap(textureImage, normalMap, imageSize.x, imageSize.y, 16.0F);

	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), normalMap, &normalMipmapImages);
	delete[] normalMap;

	Texture *trunkNormalTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, normalMipmapImages);

	ReleaseMipmapImages(normalMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Branch diffuse texture

	ImportTargaImageFile("Textures/Branch-diff.tga", &textureImage, &imageSize);
	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *branchDiffuseTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Branch transmission-specular texture

	ImportTargaImageFile("Textures/Branch-xmit-spec.tga", &textureImage, &imageSize);
	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *branchTransmissionTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Branch normal texture

	ImportTargaImageFile("Textures/Branch-nrml.tga", &textureImage, &imageSize);
	normalMap = new Color2S[imageSize.x * imageSize.y];
	ConstructNormalMap(textureImage, normalMap, imageSize.x, imageSize.y, 16.0F);

	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), normalMap, &normalMipmapImages);
	delete[] normalMap;

	Texture *branchNormalTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, normalMipmapImages);

	ReleaseMipmapImages(normalMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Trunk shader

	File vertexShader("Shaders/Vertex.glsl");
	File trunkAmbientFragmentShader("Shaders/Trunk-Ambient.glsl");
	File trunkLightFragmentShader("Shaders/Trunk-Light.glsl");

	const char *vertexString = vertexShader.GetData();
	const char *ambientFragmentString = trunkAmbientFragmentShader.GetData();
	const char *lightFragmentString = trunkLightFragmentShader.GetData();
	Program *trunkAmbientProgram = new Program(1, &vertexString, 1, &ambientFragmentString);
	Program *trunkLightProgram = new Program(1, &vertexString, 1, &lightFragmentString);

	// Branch shader

	File branchAmbientFragmentShader("Shaders/Branch-Ambient.glsl");
	File branchLightFragmentShader("Shaders/Branch-Light.glsl");

	ambientFragmentString = branchAmbientFragmentShader.GetData();
	lightFragmentString = branchLightFragmentShader.GetData();
	Program *branchAmbientProgram = new Program(1, &vertexString, 1, &ambientFragmentString);
	Program *branchLightProgram = new Program(1, &vertexString, 1, &lightFragmentString);

	// Geometry

	Node *modelNode = OpenGexDataDescription::ImportGeometry("Models/Redwood.ogex", meshArray);
	if (modelNode)
	{
		modelNode->nodeTransform.SetTranslation(position);
		rootNode->AppendSubnode(modelNode);

		int32 geometryIndex = 0;
		for (MeshGeometry *meshGeometry : meshArray)
		{
			if (geometryIndex == 0)
			{
				meshGeometry->SetTexture(0, trunkDiffuseTexture);
				meshGeometry->SetTexture(1, trunkNormalTexture);
				meshGeometry->SetTextureCount(2);

				meshGeometry->SetProgram(0, trunkAmbientProgram);
				meshGeometry->SetProgram(1, trunkLightProgram);
			}
			else
			{
				meshGeometry->SetCullFaceFlag(false);

				meshGeometry->SetTexture(0, branchDiffuseTexture);
				meshGeometry->SetTexture(1, branchTransmissionTexture);
				meshGeometry->SetTexture(2, branchNormalTexture);
				meshGeometry->SetTextureCount(3);

				meshGeometry->SetProgram(0, branchAmbientProgram);
				meshGeometry->SetProgram(1, branchLightProgram);
			}

			geometryIndex++;
		}
	}

	branchLightProgram->Release();
	branchAmbientProgram->Release();
	trunkLightProgram->Release();
	trunkAmbientProgram->Release();
	branchNormalTexture->Release();
	branchTransmissionTexture->Release();
	branchDiffuseTexture->Release();
	trunkNormalTexture->Release();
	trunkDiffuseTexture->Release();
}

void WorldManager::BuildGoblin(const Point3D& position)
{
	Array<MeshGeometry *>	meshArray;
	Color4U					*textureImage;
	Color4U					*colorMipmapImages;
	Color2S					*normalMipmapImages;
	Integer2D				imageSize;

	// Diffuse texture

	ImportTargaImageFile("Textures/Goblin-diff.tga", &textureImage, &imageSize);
	int32 mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *goblinDiffuseTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Specular texture

	ImportTargaImageFile("Textures/Goblin-spec.tga", &textureImage, &imageSize);
	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *goblinSpecularTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Normal texture

	ImportTargaImageFile("Textures/Goblin-nrml.tga", &textureImage, &imageSize);
	Color2S *normalMap = new Color2S[imageSize.x * imageSize.y];
	ConstructNormalMap(textureImage, normalMap, imageSize.x, imageSize.y, 16.0F);

	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), normalMap, &normalMipmapImages);
	delete[] normalMap;

	Texture *goblinNormalTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, normalMipmapImages);

	ReleaseMipmapImages(normalMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Shaders

	File vertexShader("Shaders/Vertex.glsl");
	File ambientFragmentShader("Shaders/Goblin-Ambient.glsl");
	File lightFragmentShader("Shaders/Goblin-Light.glsl");

	const char *vertexString = vertexShader.GetData();
	const char *ambientFragmentString = ambientFragmentShader.GetData();
	const char *lightFragmentString = lightFragmentShader.GetData();
	Program *ambientProgram = new Program(1, &vertexString, 1, &ambientFragmentString);
	Program *lightProgram = new Program(1, &vertexString, 1, &lightFragmentString);

	File eyeAmbientFragmentShader("Shaders/Eye-Ambient.glsl");
	File eyeLightFragmentShader("Shaders/Eye-Light.glsl");

	ambientFragmentString = eyeAmbientFragmentShader.GetData();
	lightFragmentString = eyeLightFragmentShader.GetData();
	Program *eyeAmbientProgram = new Program(1, &vertexString, 1, &ambientFragmentString);
	Program *eyeLightProgram = new Program(1, &vertexString, 1, &lightFragmentString);

	// Geometry

	Node *modelNode = OpenGexDataDescription::ImportGeometry("Models/Goblin.ogex", meshArray);
	if (modelNode)
	{
		modelNode->nodeTransform.SetTranslation(position);
		Vector3D v = Normalize(position);
		modelNode->nodeTransform[0] = -v;
		modelNode->nodeTransform[1].Set(v.y, -v.x, 0.0F);

		rootNode->AppendSubnode(modelNode);

		int32 geometryIndex = 0;
		for (MeshGeometry *meshGeometry : meshArray)
		{
			if (geometryIndex == 0)
			{
				meshGeometry->SetTexture(0, goblinDiffuseTexture);
				meshGeometry->SetTexture(1, goblinSpecularTexture);
				meshGeometry->SetTexture(2, goblinNormalTexture);
				meshGeometry->SetTextureCount(3);

				meshGeometry->SetProgram(0, ambientProgram);
				meshGeometry->SetProgram(1, lightProgram);

				meshGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
				meshGeometry->fragmentParam[1].Set(1.0F, 1.0F, 1.0F, 100.0F);
				meshGeometry->SetFragmentParamLocation(32);
				meshGeometry->SetFragmentParamCount(2);
			}
			else
			{
				meshGeometry->SetProgram(0, eyeAmbientProgram);
				meshGeometry->SetProgram(1, eyeLightProgram);

				meshGeometry->fragmentParam[0].Set(0.01F, 0.01F, 0.01F, 1.0F);
				meshGeometry->fragmentParam[1].Set(0.5F, 0.5F, 0.5F, 300.0F);
				meshGeometry->SetFragmentParamLocation(32);
				meshGeometry->SetFragmentParamCount(2);
			}

			geometryIndex++;
		}

		// Move a couple bones out of the bind pose.

		Node *bone = modelNode->FindNode("Goblin L UpperArm");
		bone->nodeTransform = bone->nodeTransform * Matrix3D::MakeRotationY(-1.2F);

		bone = modelNode->FindNode("Goblin R UpperArm");
		bone->nodeTransform = bone->nodeTransform * Matrix3D::MakeRotationY(1.2F);
	}

	eyeLightProgram->Release();
	eyeAmbientProgram->Release();
	lightProgram->Release();
	ambientProgram->Release();
	goblinNormalTexture->Release();
	goblinSpecularTexture->Release();
	goblinDiffuseTexture->Release();
}

void WorldManager::BuildWorld(void)
{
	Color4U			*textureImage;
	Color4U			*colorMipmapImages;
	Color4U         *horizonMipmapImages;
	Color2S			*normalMipmapImages;
	Integer2D		imageSize;



	// Load the stone wall texture, create its mipmaps, and create a Texture object for it.

	ImportTargaImageFile("Textures/StoneWall-diff.tga", &textureImage, &imageSize);
	int32 mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), textureImage, &colorMipmapImages);

	Texture *wallDiffuseTexture = new Texture(Texture::kType2D, Texture::kFormatGammaRgba, imageSize.x, imageSize.y, 1, mipmapCount, colorMipmapImages);

	ReleaseMipmapImages(colorMipmapImages);
	ReleaseTargaImageData(textureImage);

	// Load the height map for the stone wall.

	ImportTargaImageFile("Textures/StoneWall-nrml.tga", &textureImage, &imageSize);
	Color2S *normalMap = new Color2S[imageSize.x * imageSize.y];
	ConstructNormalMap(textureImage, normalMap, imageSize.x, imageSize.y, 16.0F);

	mipmapCount = GenerateMipmapImages(Integer3D(imageSize, 1), normalMap, &normalMipmapImages);
	delete[] normalMap;

	Texture *wallNormalTexture = new Texture(Texture::kType2D, Texture::kFormatSignedRedGreen, imageSize.x, imageSize.y, 1, mipmapCount, normalMipmapImages);

	ReleaseMipmapImages(normalMipmapImages);
	ReleaseTargaImageData(textureImage);

	//horizon map according to listing 7.11
	Color4U* texel = new Color4U[1536];
	GenerateHorizonCube(texel);
	


	// Load the source for the vertex and fragment shaders, and create Program objects for them.

	File vertexShader("Shaders/Vertex.glsl");
	File ambientFragmentShader("Shaders/Stone-Ambient.glsl");
	File lightFragmentShader("Shaders/Stone-Light.glsl");

	const char *vertexString = vertexShader.GetData();
	const char *ambientFragmentString = ambientFragmentShader.GetData();
	const char *lightFragmentString = lightFragmentShader.GetData();
	Program *ambientProgram = new Program(1, &vertexString, 1, &ambientFragmentString);
	Program *lightProgram = new Program(1, &vertexString, 1, &lightFragmentString);

	// Create the room

	BuildFloor(ambientProgram, lightProgram);
	BuildWalls(ambientProgram, lightProgram, wallDiffuseTexture, wallNormalTexture);

	// Create 200 geometries at random locations inside a 50m distance from the origin.
	// The texture and programs created above are assigned to each geometry.

	for (int32 k = 0; k < 50; k++)
	{
		BoxGeometry *boxGeometry = new BoxGeometry(Vector3D(RandomFloat(0.5F, 4.0F), RandomFloat(0.5F, 4.0F), RandomFloat(0.5F, 4.0F)));
		Vector2D direction = RandomUnitVector2D();
		boxGeometry->nodeTransform.Set(RandomUnitQuaternion().GetRotationMatrix(), Point3D(direction * (RandomFloat(20.0F) + 20.0F), RandomFloat(20.0F)));
		rootNode->AppendSubnode(boxGeometry);

		boxGeometry->SetTexture(0, wallDiffuseTexture);
		boxGeometry->SetTexture(1, wallNormalTexture);
		boxGeometry->SetTextureCount(2);

		boxGeometry->SetProgram(0, ambientProgram);
		boxGeometry->SetProgram(1, lightProgram);

		boxGeometry->fragmentParam[0].Set(RandomFloat(0.875F) + 0.125F, RandomFloat(0.875F) + 0.125F, RandomFloat(0.875F) + 0.125F, 1.0F);
		boxGeometry->fragmentParam[1].Set(1.0F, 1.0F, 1.0F, 100.0F);
		boxGeometry->SetFragmentParamLocation(32);
		boxGeometry->SetFragmentParamCount(2);
	}

	for (int32 k = 0; k < 10; k++)
	{
		SphereGeometry *sphereGeometry = new SphereGeometry(RandomFloat(0.5F, 4.0F));
		Vector2D direction = RandomUnitVector2D();
		sphereGeometry->nodeTransform.SetTranslation(Point3D(direction * (RandomFloat(20.0F) + 20.0F), RandomFloat(20.0F)));
		rootNode->AppendSubnode(sphereGeometry);

		sphereGeometry->SetTexture(0, wallDiffuseTexture);
		sphereGeometry->SetTexture(1, wallNormalTexture);
		sphereGeometry->SetTextureCount(2);

		sphereGeometry->SetProgram(0, ambientProgram);
		sphereGeometry->SetProgram(1, lightProgram);

		sphereGeometry->fragmentParam[0].Set(RandomFloat(0.875F) + 0.125F, RandomFloat(0.875F) + 0.125F, RandomFloat(0.875F) + 0.125F, 1.0F);
		sphereGeometry->fragmentParam[1].Set(1.0F, 1.0F, 1.0F, 100.0F);
		sphereGeometry->SetFragmentParamLocation(32);
		sphereGeometry->SetFragmentParamCount(2);
	}

	BuildTree(Point3D(5.0F, 5.0F, 0.0F));
	BuildGoblin(Point3D(5.0F, -5.0F, 0.0F));

	// We can release our local references to the textures and programs here because they are
	// still referenced by all of the geometries and will not be deleted.

	lightProgram->Release();
	ambientProgram->Release();
	wallNormalTexture->Release();
	wallDiffuseTexture->Release();

	// Set the ambient light color and add some point lights to the world.

	ambientColor.Set(0.0625F, 0.0625F, 0.0625F, 0.0F);

	File constantFragmentShader("Shaders/Constant.glsl");
	const char *constantFragmentString = constantFragmentShader.GetData();
	Program *constantProgram = new Program(1, &vertexString, 1, &constantFragmentString);

	// Stationary light

	PointLight *pointLight = new PointLight(ColorRgba(5.0F, 5.0F, 5.0F), 100.0F);
	pointLight->nodeTransform.SetTranslation(Point3D(0.0F, 0.0F, 20.0F));
	rootNode->AppendSubnode(pointLight);

	SphereGeometry *sphereGeometry = new SphereGeometry(0.125F);
	sphereGeometry->SetProgram(0, constantProgram);
	pointLight->AppendSubnode(sphereGeometry);

	sphereGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
	sphereGeometry->SetFragmentParamLocation(32);
	sphereGeometry->SetFragmentParamCount(1);

	// Moving lights

	for (int32 k = 0; k < 4; k++)
	{
		pointLight = new PointLight(ColorRgba(5.0F, 5.0F, 5.0F), 25.0F);
		pointLight->nodeTransform[3] = RandomPointInsideSphere(25.0F);
		pointLight->nodeController = new LightController(pointLight);
		rootNode->AppendSubnode(pointLight);

		sphereGeometry = new SphereGeometry(0.125F);
		sphereGeometry->SetProgram(0, constantProgram);
		pointLight->AppendSubnode(sphereGeometry);

		sphereGeometry->fragmentParam[0].Set(1.0F, 1.0F, 1.0F, 1.0F);
		sphereGeometry->SetFragmentParamLocation(32);
		sphereGeometry->SetFragmentParamCount(1);
	}

	constantProgram->Release();
}
