#ifndef _ROS_SERVICE_EstimateHomography_h
#define _ROS_SERVICE_EstimateHomography_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "sensor_msgs/Image.h"

namespace ground_projection
{

static const char ESTIMATEHOMOGRAPHY[] = "ground_projection/EstimateHomography";

  class EstimateHomographyRequest : public ros::Msg
  {
    public:
      sensor_msgs::Image image;

    EstimateHomographyRequest():
      image()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->image.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->image.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return ESTIMATEHOMOGRAPHY; };
    const char * getMD5(){ return "b13d2865c5af2a64e6e30ab1b56e1dd5"; };

  };

  class EstimateHomographyResponse : public ros::Msg
  {
    public:
      uint8_t homography_length;
      float st_homography;
      float * homography;

    EstimateHomographyResponse():
      homography_length(0), homography(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset++) = homography_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < homography_length; i++){
      union {
        float real;
        uint32_t base;
      } u_homographyi;
      u_homographyi.real = this->homography[i];
      *(outbuffer + offset + 0) = (u_homographyi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_homographyi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_homographyi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_homographyi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->homography[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint8_t homography_lengthT = *(inbuffer + offset++);
      if(homography_lengthT > homography_length)
        this->homography = (float*)realloc(this->homography, homography_lengthT * sizeof(float));
      offset += 3;
      homography_length = homography_lengthT;
      for( uint8_t i = 0; i < homography_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_homography;
      u_st_homography.base = 0;
      u_st_homography.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_homography.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_homography.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_homography.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_homography = u_st_homography.real;
      offset += sizeof(this->st_homography);
        memcpy( &(this->homography[i]), &(this->st_homography), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return ESTIMATEHOMOGRAPHY; };
    const char * getMD5(){ return "5891e13cbfb0399924e8d5e7e53d283a"; };

  };

  class EstimateHomography {
    public:
    typedef EstimateHomographyRequest Request;
    typedef EstimateHomographyResponse Response;
  };

}
#endif
