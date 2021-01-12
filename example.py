import torch
from kmeans_pytorch import kmeans
import open3d as o3d
import numpy as np

# data
data_size, dims, num_clusters = 1000, 2, 10

cloud = o3d.io.read_point_cloud("/home/llg/dataset_paper/camp001_l3.ply")
cloud_xyz=np.array(cloud.points)
x = torch.from_numpy(cloud_xyz)

# kmeans
cluster_ids_x, cluster_centers = kmeans(
    X=x, num_clusters=num_clusters, distance='euclidean', device=torch.device('cuda:0')
)