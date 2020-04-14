#pragma once
#include <costmap_converter/ObstacleArrayMsg.h>
#include <geometry_msgs/PointStamped.h>
#include <ros/ros.h>
#include <visualization_msgs/MarkerArray.h>
// #include <costmap_2d/costmap_2d.h>
// #include <costmap_2d/costmap_2d_ros.h>
#include <geometry_msgs/Polygon.h>
#include <geometry_msgs/PolygonStamped.h>

#include <deque>
#include <set>
#include <vector>

#include "planner/Node2d.h"
#include "planner/Node3d.h"

// struct GridAStartResult {
//   std::vector<double> x;
//   std::vector<double> y;
//   double path_cost = 0.0;
// };

class GridMap {
 public:
  GridMap() {
    pub_map =
        nh.advertise<visualization_msgs::MarkerArray>("vis_heuristic_map", 10);
    pub_border =
        nh.advertise<visualization_msgs::MarkerArray>("vis_border", 10);
    pub_obstacle =
        nh.advertise<visualization_msgs::MarkerArray>("vis_obstacle", 10);
  };
  ~GridMap() = default;

  std::shared_ptr<Node2d> CreateNodeFromWorldCoord(double x, double y);
  std::shared_ptr<Node2d> CreateNodeFromGridCoord(int x, int y);
  std::shared_ptr<Node2d> GetNodeFromWorldCoord(double x, double y);
  std::shared_ptr<Node2d> GetNodeFromGridCoord(int x_grid, int y_grid);
  bool GenerateDestinationDistanceMap();
  double GetHeuristic(std::string s);

  bool GenerateObstacleDistanceMap();
  bool SetXYResolution(double resolution);
  bool SetStartPoint(double x, double y);
  bool SetEndPoint(double x, double y);
  bool SetBounds(double xmin, double xmax, double ymin, double ymax);
  void AddObstacles(double left, double right, double up, double bottom);
  void AddPolygonObstacles(geometry_msgs::Polygon p);
  void ClearMap();
  void ClearObstacles();
  void PlotHeuristicMap(double xy_grid_resolution);
  void PlotBorders(double xy_grid_resolution);
  void PlotObstacleMap(double xy_grid_resolution);

  std::unordered_map<std::string, std::shared_ptr<Node2d>> heuristic_map_;

 private:
  double EuclidDistance(const double x1, const double y1, const double x2,
                        const double y2);
  std::vector<std::shared_ptr<Node2d>> GenerateNextNodes(
      std::shared_ptr<Node2d> node);
  bool CheckConstraints(std::shared_ptr<Node2d> node);
  bool InsideMapRange(const int node_grid_x, const int node_grid_y);

  double xy_grid_resolution_ = 0.3;
  double phi_grid_resolution_ = 0.2;
  double node_radius_ = 0.0;
  std::vector<double> XYbounds_{0, 10, 0, 10};
  double max_grid_x_ = 0.0;
  double max_grid_y_ = 0.0;
  std::shared_ptr<Node2d> start_node_ = nullptr;
  std::shared_ptr<Node2d> end_node_ = nullptr;

  std::vector<std::vector<int>> grid_obstacles_;

  visualization_msgs::MarkerArray marker_array;
  visualization_msgs::Marker marker;

  ros::NodeHandle nh;
  ros::Publisher pub_map, pub_border, pub_obstacle;

  double max_cost = std::numeric_limits<double>::min();
  std::set<std::string> border_available_;
  std::set<std::string> border_unavailable_;
};
