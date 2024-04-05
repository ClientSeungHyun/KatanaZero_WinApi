#include "RigidBody.h"
#include "Player.h"

#include "TimeMgr.h"

CRigidBody::CRigidBody() :m_pOwner(nullptr), m_fMass(1.f), m_fFriction(0.f), m_vMaxVelocity(VEC2(100.f, 100.f)), m_bIsGround(false), m_bIsSlope(false)
, m_fGravityAccel(2.f), m_fGravityScale(10.f), m_bIsGravity(true), m_bIsWallGrab(false)
{
}

CRigidBody::CRigidBody(const CRigidBody& _rhs)
{

}

CRigidBody::~CRigidBody()
{
}

void		CRigidBody::Initialize()
{
	m_vMaxVelocity={100.f,100.f};
	m_fGravityAccel = 2.f;
	m_fGravityScale = 10.f;
}

void		CRigidBody::Late_Update()
{
	//�������� ���� ���� ���� ����
	//���� ���� �ӵ��� ����
	if (m_vForce.Length() != 0.f)
	{
		//���ӵ��� = �� / ����
		m_vAccel = m_vForce / m_fMass;

		//���ӵ��� �̿��ؼ� �ӵ��� ������Ų��.
		m_vVelocity += m_vAccel * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale();
	}

	//�߷� ����
	if (m_bIsFall && m_pOwner->Get_Id() == OBJ_PLAYER && !g_bIsTimeBack )
	{
		VEC2 vGravityAccel = { 0.f, m_fGravityAccel * 10.0f * CTimeMgr::Get_Instance()->Get_TimeScale() };

		m_vVelocity += vGravityAccel;
	}

	//�߷� ����
	if (!m_bIsGround && m_pOwner->Get_Id() == OBJ_ENEMY)
	{
		VEC2 vGravityAccel = { 0.f, m_fGravityAccel * m_fGravityScale * CTimeMgr::Get_Instance()->Get_TimeScale() };

		m_vVelocity += vGravityAccel;
	}

	if (!m_bIsGround && m_pOwner->Get_Id() == OBJ_HEAD)
	{
		VEC2 vGravityAccel = { 0.f, m_fGravityAccel * 30.f * CTimeMgr::Get_Instance()->Get_TimeScale() };

		m_vVelocity += vGravityAccel;
	}

	Move();

	//�� �ʱ�ȭ
	m_vForce = { 0.f, 0.f };
}

void		CRigidBody::CalcFriction()
{
	if (!m_vVelocity.IsZero())
	{
		VEC2 vFricitionDir = m_vVelocity;
		vFricitionDir.Normalize();

		VEC2 vFriction = -vFricitionDir.Normalize() * m_fFriction * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale();

		//������ �� ū ���
		if (vFriction.Length() >= m_vVelocity.Length())
		{
			m_vVelocity = { 0.f,0.f };
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}
}

//�÷��̾� ������
void		CRigidBody::Move()
{
	VEC2 vPos = m_vVelocity * CTimeMgr::Get_Instance()->Get_DeltaTime() * CTimeMgr::Get_Instance()->Get_TimeScale();

	//�ӵ� ����
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}
	if ((m_vMaxVelocity.y) < (m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}


	m_pOwner->Set_Pos(m_pOwner->Get_Pos().x + vPos.x, m_pOwner->Get_Pos().y + vPos.y);
}
