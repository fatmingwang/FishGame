#ifndef _BOUND_H
#define _BOUND_H

#include "Collision.h"

	struct EnumStringMap
	{
		DWORD Value;
		const WCHAR* szName;
	};

    //class cBound
    //{
    //public:
    //    enum BoundType
    //    {
    //        No_Bound,
    //        Sphere_Bound,
    //        Rectangle_Bound,
    //        ForceDWORD = 0x7FFFFFFF
    //    };
    //    bool                    Collide( const cBound& Other ) const;
    //    bool                    Collide( sSphere e_Sphere ) const;
    //    bool                    Collide( RECT e_Rect ) const;
    //    BoundType               GetType() const { return m_Type; }
    //    const sSphere&           GetSphere() const;
    //    void                    SetSphere( const sSphere& Sphere );
    //    const sFrustum&          GetFrustum() const;
    //    void                    SetFrustum( const sFrustum& Frustum );
    //    const sAxisAlignedBox&   GetAabb() const;
    //    void                    SetAabb( const sAxisAlignedBox& Aabb );
    //    const sOrientedBox&      GetObb() const;
    //    void                    SetObb( const sOrientedBox& Obb );
    //private:
    //    BoundType       m_Type;
    //    // Data must be big enough to hold the union of all the bound types.
    //    //BYTE            m_Data[ sizeof( FLOAT ) * 13 ];
    //    BYTE            m_Data[ sizeof( FLOAT ) * 4 ];
    //};

    class cBound
    {
    public:
        enum BoundType
        {
            No_Bound,
            Sphere_Bound,
            Frustum_Bound,
            OBB_Bound,
            AABB_Bound,
            ForceDWORD = 0x7FFFFFFF
        };

        static const EnumStringMap   BoundType_StringMap[];

        // construct/destruct a bound
        cBound() { Clear(); }
        cBound( const sSphere& Sphere ) { SetSphere( Sphere ); }
        cBound( const sFrustum& Frustum ){ SetFrustum( Frustum ); }
        cBound( const sAxisAlignedBox& Aabb ){ SetAabb( Aabb ); }
        cBound( const sOrientedBox& Obb ){ SetObb( Obb ); }
        cBound( const cBound& Other ){ *this = Other; }
        virtual ~cBound() {}

        VOID Clear() { m_Type = cBound::No_Bound; }

        // collision with other primitives
        bool            Collide( const cBound& Other ) const;
        bool            Collide( const sFrustum& Frustum ) const;
        bool            Collide( const sSphere& Sphere ) const;
        bool            Collide( const sAxisAlignedBox& Aabb ) const;
        bool            Collide( const sOrientedBox& pObb ) const;

        // merge with another bound
        void            Merge( const cBound& Other );

        // transformation
        cBound           operator*( const cMatrix44 Matrix ) const;

        // get-sets
        // Note that the get functions will assert if you try to
        // get a type not equal to the bounds current BoundType
        BoundType                GetType() const { return m_Type; }
        const sSphere&           GetSphere() const;
        void                     SetSphere( const sSphere& Sphere );
        const sFrustum&          GetFrustum() const;
        void                     SetFrustum( const sFrustum& Frustum );
        const sAxisAlignedBox&   GetAabb() const;
        void                     SetAabb( const sAxisAlignedBox& Aabb );
        const sOrientedBox&      GetObb() const;
        void                     SetObb( const sOrientedBox& Obb );

        // gets the center of the bound. 
        Vector3					 GetCenter() const;

        // gets the bound's maximum radius
        float					GetMaxRadius() const;

    private:
        BoundType       m_Type;
        
        // Data must be big enough to hold the union of all the bound types.
        BYTE            m_Data[ sizeof( FLOAT ) * 13 ];

    };

#endif