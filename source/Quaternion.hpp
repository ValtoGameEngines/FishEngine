#ifndef Quaternion_hpp
#define Quaternion_hpp

#include "Vector3.hpp"
#define GLM_FORCE_LEFT_HANDED
//#include <glm/glm.hpp>
//#include <glm/vec3.hpp>                 // glm::vec3
//#include <glm/vec4.hpp>                 // glm::vec4
//#include <glm/mat4x4.hpp>               // glm::mat4
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//#include <glm/gtc/constants.hpp>        // glm::pi
#include <glm/gtx/quaternion.hpp>       // glm::quat
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

class Quaternion
{
public:
	union {
        struct { float x, y, z, w; };
        float m[4];
    };

    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Quaternion() : Quaternion(0, 0, 0, 1) {}

    Quaternion(const glm::quat& glm_quat) : x(glm_quat.x), y(glm_quat.y), z(glm_quat.z), w(glm_quat.w) {

    }

    operator glm::quat() const {
        return glm::quat(w, x, y, z);
    }

    Vector3 eulerAngles() const
    {
    	//return Vector3(pitch(), yaw(), roll());
        return glm::eulerAngles(glm::quat(w, x, y, z));
    }

    float roll() const {
    	return Mathf::Rad2Deg * ::std::atan2(2.0f * (z*y+w*z), w*w+x*x-y*y-z*z);
    }

    float pitch() const {
		return Mathf::Rad2Deg * ::std::atan2(2.0f * (y*z+w*x), w*w-x*x-y*y+z*z);
    }

    float yaw() const {
    	return Mathf::Rad2Deg * ::std::asin(Mathf::Clamp(-2.f*(x*z-w*y), -1.f, 1.f));
    }
    
    void setEulerAngles(const Vector3& angles)
    {
    	*this = Quaternion::Euler(angles);
    }

    void Set(float new_x, float new_y, float new_z, float new_w)
    {
        x = new_x;
        y = new_y;
        z = new_z;
        w = new_w;
    }

    float operator[](int index) const {
        return m[index];
    }

    float& operator[](int index) {
        return m[index];
    }

    friend Quaternion operator *(Quaternion lhs, Quaternion rhs)
    {
        return Quaternion(lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y, lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z, lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x, lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
    }

    friend Vector3 operator *(Quaternion rotation, Vector3 point)
    {
        float num = rotation.x * 2.f;
        float num2 = rotation.y * 2.f;
        float num3 = rotation.z * 2.f;
        float num4 = rotation.x * num;
        float num5 = rotation.y * num2;
        float num6 = rotation.z * num3;
        float num7 = rotation.x * num2;
        float num8 = rotation.x * num3;
        float num9 = rotation.y * num3;
        float num10 = rotation.w * num;
        float num11 = rotation.w * num2;
        float num12 = rotation.w * num3;
        Vector3 result;
        result.x = (1.f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
        result.y = (num7 + num12) * point.x + (1.f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
        result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.f - (num4 + num5)) * point.z;
        return result;
    }


    static float Angle(const Quaternion& a, const Quaternion& b)
    {
        float f = Quaternion::Dot(a, b);
        return Mathf::Acos(Mathf::Min(Mathf::Abs(f), 1.f)) * 2.f * 57.29578f;
    }

    static Quaternion AngleAxis(float angle, const Vector3& axis)
    {
        return glm::angleAxis(Mathf::Radians(angle), (glm::vec3)axis.normalized());
        //Quaternion Result;

        //float s = ::std::sin(angle * 0.5f);

        //Result.w = ::std::cos(angle * 0.5f);
        //Result.x = axis.x * s;
        //Result.y = axis.y * s;
        //Result.z = axis.z * s;
        //return Result;
    }

    void ToAngleAxis(float* angle, Vector3* axis);

    static float Dot(const Quaternion& a, const Quaternion& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static Quaternion Euler(float x, float y, float z)
    {
        return Euler(Vector3(x, y, z));
    }

    static Quaternion Euler(const Vector3& euler)
    {
        return glm::quat(euler);
        //Quaternion result;
        //auto rad = euler * (Mathf::Deg2Rad * 0.5f);
        //auto c = Vector3(cos(rad.x), cos(rad.y), cos(rad.z));
        //auto s = Vector3(sin(rad.x), sin(rad.y), sin(rad.z));
        //result.w = c.x * c.y * c.z + s.x * s.y * s.z;
        //result.x = s.x * c.y * c.z - c.x * s.y * s.z;
        //result.y = c.x * s.y * c.z + s.x * c.y * s.z;
        //result.z = c.x * c.y * s.z - s.x * s.y * c.z;
        //return result;
    }

    static Quaternion FromToRotation(const Vector3& fromDirection, const Vector3& toDirection);
    
    void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection)
    {
        *this = Quaternion::FromToRotation(fromDirection, toDirection);
    }


    static Quaternion Inverse(const Quaternion& rotation)
    {
        float num = 1.0f / Dot(rotation, rotation);
        return Quaternion(-rotation.x * num, -rotation.y * num, -rotation.z*num, -rotation.w*num);
    }

    static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion LerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t);
    static Quaternion SlerpUnclamped(const Quaternion& a, const Quaternion& b, float t);

    static Quaternion LookRotation(const Vector3& forward, const Vector3& upwards);
    
    void SetLookRotation(const Vector3& view)
    {
        Vector3 up = Vector3::up;
        SetLookRotation(view, up);
    }

    void SetLookRotation(const Vector3& view, const Vector3& up)
    {
        *this = Quaternion::LookRotation(view, up);
    }

    static Quaternion LookRotation(const Vector3& forward);

    static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
    {
        float num = Quaternion::Angle(from, to);
        if (num == 0.f)
        {
            return to;
        }
        float t = Mathf::Min(1.f, maxDegreesDelta / num);
        return Quaternion::SlerpUnclamped(from, to, t);
    }




    static const Quaternion identity;
};

#endif // Quaternion_hpp