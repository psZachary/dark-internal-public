#include "pch.h"
#include <util.h>
#include "Overlay/overlay.h"


#define M_PI 3.14159265358979323846264338327950288

#define convert_vector(sdk_vector) vector2{ (float)sdk_vector.X, (float)sdk_vector.Y }

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;

static D3DMATRIX matrix(CG::FRotator rot, CG::FVector origin = CG::FVector(0, 0, 0))
{
	float radPitch = (rot.Roll * float(M_PI) / 180.f);
	float radYaw = (rot.Yaw * float(M_PI) / 180.f);
	float radRoll = (rot.Pitch * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.X;
	matrix.m[3][1] = origin.Y;
	matrix.m[3][2] = origin.Z;
	matrix.m[3][3] = 1.f;

	return matrix;
}


static bool w2s(CG::FVector location, CG::FMinimalViewInfo camera_cache, vector2 screen_loc)
{

	auto POV = camera_cache;
	CG::FRotator Rotation = POV.Rotation;
	D3DMATRIX tempMatrix = matrix(Rotation);

	CG::FVector vAxisX, vAxisY, vAxisZ;

	vAxisX = CG::FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = CG::FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = CG::FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	CG::FVector vDelta = location - POV.Location;
	CG::FVector vTransformed = CG::FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.Z < 1.f)
		return false;

	float FovAngle = POV.FOV;

	screen_loc.x = (util::screen_size().x / 2) + vTransformed.X * ((util::screen_size().x / 2) / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;
	screen_loc.x = (util::screen_size().y / 2) - vTransformed.Y * ((util::screen_size().x / 2) / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.Z;

	if (screen_loc.x > util::screen_size().x ||
		screen_loc.y > util::screen_size().y ||
		screen_loc.y < 0 ||
		screen_loc.x < 0) return false;

	return true;
}

namespace features {
	void target_loot_esp(Overlay* overlay);
	void loot_esp(Overlay* overlay);
	void player_esp(Overlay* overlay);
}