#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    
    start_node = &(m_Model.FindClosestNode(start_x, start_y));
    end_node = &(m_Model.FindClosestNode(end_x, end_y));
}



// - Node objects have a distance method to determine the distance to another node.


float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
//
return node->distance( *(this->end_node) );
// float distance = node->distance(*end_node); //this would have also worked

// return distance;
}



void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    
    current_node->RouteModel::Node::FindNeighbors(); //What to pass as an argument
    for(RouteModel::Node* neighbor : current_node->neighbors){
        neighbor->parent = current_node;//got it
        neighbor->g_value = (current_node->g_value) + current_node->distance(*neighbor);//got it
        neighbor->h_value = this->RoutePlanner::CalculateHValue(neighbor);
        this->open_list.push_back(neighbor);
        neighbor->visited = true;
    }
}



// compare function for sort
bool compare(const RouteModel::Node *n1, const RouteModel::Node *n2) { 
	return n1->h_value + n1->g_value < n2->h_value + n2->g_value;
}

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), compare);
  
  	RouteModel::Node *optimal_node = open_list.front();
  	open_list.erase(open_list.begin());
  	
  	return optimal_node;
}




std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
        distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
  	while (current_node->parent != nullptr) {
		distance += current_node->distance(*current_node->parent);
      	path_found.insert(path_found.begin(), *current_node);
      	current_node = current_node->parent;
    }
  	
  	path_found.insert(path_found.begin(), *current_node);
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}




void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
  	start_node->visited = true;
  	open_list.push_back(start_node);
  
    while (open_list.size() > 0) {
        current_node = NextNode();
     
        if (current_node->distance(*end_node) == 0) {
            m_Model.path = ConstructFinalPath(current_node);
          	return;
        } 
      
      	AddNeighbors(current_node); 
    }

}