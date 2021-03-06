#ifndef ASLAM_PYTHON_EXPORT_NULL_UNDISTORTER_HPP
#define ASLAM_PYTHON_EXPORT_NULL_UNDISTORTER_HPP

#include <aslam/NullUndistorter.hpp>
#include <Eigen/Core>
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <numpy_eigen/boost_python_headers.hpp>

namespace aslam {
namespace cameras {

typedef Eigen::Matrix<boost::uint8_t, Eigen::Dynamic, Eigen::Dynamic> image_t;

template<typename DISTORTION_T>
image_t undistortImageNumpyCvmat(const NullUndistorter<DISTORTION_T> *instance, const image_t & image_eigen)
{
  cv::Mat image_cv, image_cv_undist;
  eigen2cv(image_eigen, image_cv);

  instance->undistortImage(image_cv, image_cv_undist);

  image_t image_eigen_undist(image_cv_undist.rows, image_cv_undist.cols);
  cv2eigen(image_cv_undist, image_eigen_undist);

  return image_eigen_undist;
}

template<typename DISTORTION_T>
void exportNullUndistorter(const std::string & name) {
  using namespace boost::python;
  typedef DISTORTION_T distortion_t;
  typedef typename NullUndistorter<distortion_t>::distorted_geometry_t distorted_geometry_t;

  class_<NullUndistorter<distortion_t>,
      boost::shared_ptr<NullUndistorter<distortion_t> > >(name.c_str(), init<>())
      .def(init<const sm::PropertyTree &, const sm::PropertyTree &>(
          (name + "(undistorterConfig, cameraConfig)").c_str()))
      .def(init<const sm::PropertyTree &, int, double, double>(
          (name + "(cameraConfig, interpolation, alpha, scale)").c_str()))
      .def(init<boost::shared_ptr<distorted_geometry_t>, int, double, double>(
          (name + "(distortedGeometry, interpolation, alpha, scale)").c_str()))
      .def("init", &NullUndistorter<distortion_t>::init)
      .def("constructUndistortedFrame", &NullUndistorter<distortion_t>::constructUndistortedFrame)
      .def("undistortImage", &NullUndistorter<distortion_t>::undistortImage)
      .def("undistortImage", &undistortImageNumpyCvmat<distortion_t>)
      .def("getIdealGeometry", &NullUndistorter<distortion_t>::idealGeometry);
}

}
}

#endif /* ASLAM_PYTHON_EXPORT_NULL_UNDISTORTER_HPP */
