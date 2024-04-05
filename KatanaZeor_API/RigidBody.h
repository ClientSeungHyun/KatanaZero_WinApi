#pragma once
#include "Obj.h"

class CRigidBody 
{
public:
    CRigidBody();
    CRigidBody(const CRigidBody& _rhs);
    ~CRigidBody();

private:
    CObj* m_pOwner;

public:
    CObj*       Get_Owner() { return m_pOwner; };

    bool        Get_IsGround() { return m_bIsGround; };
    void        Set_IsGround(bool _b) {
        m_bIsGround = _b;
       
    }

    void        Set_IsFall(bool _b) { m_bIsFall = _b; }

    bool                Get_IsSlope() { return m_bIsSlope; }
    void                Set_IsSlope(bool _b) { m_bIsSlope = _b; }

    bool                Get_IsWallGrab() { return m_bIsWallGrab; }
    void                Set_IsWallGrab(bool _b) { m_bIsWallGrab = _b; }

    bool                IsMovingInput() { return m_bIsMovingInput; }
    void                Set_MovingInput(bool _b) { m_bIsMovingInput = _b; }

    VEC2                Get_RampNormal() { return m_vecRampNormal; }
    void                Set_RampNormal(VEC2 _vecNormal) { m_vecRampNormal = _vecNormal; }

    float               Get_Mass() { return m_fMass; }
    void                Set_Mass(float _f) { m_fMass = _f; }

    void                AddForce(VEC2 _vForce) { m_vForce += _vForce; }
    VEC2                Get_Force() { return m_vForce; }

    VEC2                Get_Velocity() { return m_vVelocity; }
    void                Set_Velocity(VEC2 _vVelocity) { m_vVelocity = _vVelocity; }
    void                Set_VelocityX(float _f) { m_vVelocity.x = _f; }
    void                Set_VelocityY(float _f) { m_vVelocity.y = _f; }

    void                AddVelocity(VEC2 _vAddV) { m_vVelocity += _vAddV; }

    bool                Get_IsGravity() { return m_bIsGravity; }
    void                Set_IsGravity(bool _b) { m_bIsGravity = _b; }

    void                Set_VelocityLimit(VEC2 _vMaxVelocity) { m_vMaxVelocity = _vMaxVelocity; }
    void                Set_VelocityLimitX(float _x) { m_vMaxVelocity.x = _x; }
    void                Set_VelocityLimitY(float _y) { m_vMaxVelocity.y = _y; }

    void                Set_Friction(float _f) { m_fFriction = _f; }

    void                Set_GravityLimit(float _fLimit) { m_fGravityLimit = _fLimit; }


    void                ResetGravityAccel() { m_fGravityAccel = 2.f; }

public:
    void                Initialize();
    void                Late_Update();

public:
    void                CalcFriction();
    void                Move();

    friend class CObj;

private:
    float               m_fMass;            // 질량
    VEC2                m_vAccel;           //가속도
    VEC2                m_vVelocity;        // 속도
    VEC2                m_vForce;           // 가해지는 힘
    VEC2                m_vMaxVelocity;     // 최고 속도
    float               m_fFriction;        // 마찰력
    float               m_fSlopeAngle;      // 지면의 기울기

    VEC2                m_vecRampNormal;    // 경사로에 있을 때, 경사로의 수직인 법선

    bool                m_bIsGravity;
    bool                m_bIsGround;
    bool                m_bIsFall;
    bool                m_bIsSlope;
    bool                m_bIsWallGrab;

    bool                m_bIsMovingInput;

    float               m_fGravityScale;
    float               m_fGravityAccel;    // 중력 가속도
    float               m_fGravityLimit;   // 중력 제한용
};

