# Import libraries necessary for this project
import os
import sys
import numpy as np
import pandas as pd
import matplotlib.cm as cm
import matplotlib.pyplot as plt
from IPython.display import display # Allows the use of display() for DataFrames

# Load the wholesale customers dataset
try:
    dir = os.getcwd()
    in_file = dir + '/customers.csv'
    data = pd.read_csv(in_file)
    data.drop(['Region', 'Channel'], axis = 1, inplace = True)
    # Success - Display the first five records
    # display(data.head())
    print("Wholesale customers dataset has {} samples with {} features each.".format(*data.shape))
except:
    print("Dataset could not be loaded. Is the dataset missing?")


# Display a description of the dataset
display(data.describe())


print("\n", end='')
print("\n", end='')
# TODO: Select three indices of your choice you wish to sample from the dataset
# indices = np.random.randint(low = 0, high = 440, size = 3)  # 隨機產生0-439之間的數 取三個
indices = [231, 409, 181]
print("Random Indices: {}".format(indices))

# Create a DataFrame of the chosen samples
samples = pd.DataFrame(data.loc[indices], columns = data.keys()).reset_index(drop = True)
print("Chosen samples of wholesale customers dataset:")
display(samples)

print("Question 1:")
print("index 231: 牛奶與其他東西相比特別多，有可能是專賣牛奶的零售商(retailers)")
print("index 409: 熟食較多，且有買清潔用品，應該是餐廳(restaurant)")
print("index 181: 每一樣東西都很多，應該是超市(market)")


print("\n", end='')
print("\n", end='')
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeRegressor
# TODO: Iterate all six features
dep_vars = list(data.columns)
for var in dep_vars:
    new_data = data.copy()
    # TODO: Make a copy of the DataFrame, using the 'drop' function to drop the given feature
    target_label = new_data[var]
    new_data = data.drop([var], axis = 1)

    # TODO: Split the data into training and testing sets(0.25) using the given feature as the target
    X_train, X_test, y_train, y_test = train_test_split(new_data, target_label, test_size=0.25, random_state=0)
    regressor = DecisionTreeRegressor(random_state=0)
    regressor = regressor.fit(X_train, y_train)
    y_pred = regressor.predict(X_test)
    # TODO: Report the score of the prediction using the testing set
    score = regressor.score(X_test, y_test)
    print('R2 score for {} as dependent variable: {}'.format(var, score))


print("Question 2:")
print("Frozen，因為Frozen分數是正的但很低，代表Frozen可以fit但不好預測，有可能是重要的特徵。")
print("0.2539")
print("是，有可能是重要的特徵。")


print("\n", end='')
print("\n", end='')
# Produce a scatter matrix for each pair of features in the data
pd.plotting.scatter_matrix(data, alpha = 0.3, figsize = (14,8), diagonal = 'kde');
plt.show()

corr = data.corr()
plt.figure(figsize = (10,5))
import seaborn as sns
ax = sns.heatmap(corr, annot=True)
plt.show()

plt.figure(figsize = (20,8))
for cols in data.columns.values:
    ax = sns.kdeplot(data[cols], label=cols)
    ax.legend(loc=0, prop={'size': 15})
ax.set(xlabel=" ")
plt.show()
print("Question 3:")
print("Milk與Grocery和Detergents_Paper有高度相關。唯一與Frozen相關的也就只有Fresh，但數值不高。Delicatessen與任何東西都有一點關聯。")
print("確實符合我的想法，Frozen算是特別的特徵，沒有太多關聯性。")
print("資料不呈現常態分佈。")


print("\n", end='')
print("\n", end='')
# TODO: Scale the data using the natural logarithm
log_data = np.log(data)
# Produce a scatter matrix for each pair of newly-transformed features
pd.plotting.scatter_matrix(log_data, alpha = 0.3, figsize = (14,8), diagonal = 'kde')
plt.show()

log_samples = np.log(samples)
# Display the log-transformed sample data
display(log_samples)




# For each feature find the data points with extreme high or low values
outliers_list = []
for feature in log_data.keys():
    # TODO: Calculate Q1 (25th percentile of the data) for the given feature
    Q1 = np.percentile(log_data[feature], 25)

    # TODO: Calculate Q3 (75th percentile of the data) for the given feature
    Q3 = np.percentile(log_data[feature], 75)

    # TODO: Use the interquartile range to calculate an outlier step (1.5 times the interquartile range)
    step = (Q3 - Q1) * 1.5  # 四分位距*1.5

    # Display the outliers
    print("Data points considered outliers for the feature '{}':".format(feature))
    display(log_data[~((log_data[feature] >= Q1 - step) & (log_data[feature] <= Q3 + step))])
    print("\n", end='')
    outliers = list(log_data[~((log_data[feature] >= Q1 - step) & (log_data[feature] <= Q3 + step))].index.values)
    outliers_list.extend(outliers)


print("List of Outliers -> {}".format(outliers_list))
duplicate_outliers_list = list(set([x for x in outliers_list if outliers_list.count(x) >= 2]))
duplicate_outliers_list.sort()
print("\nList of Common Outliers -> {}".format(duplicate_outliers_list))

# OPTIONAL: Select the indices for data points you wish to remove
outliers = duplicate_outliers_list

# Remove the outliers, if any were specified
good_data = log_data.drop(log_data.index[outliers]).reset_index(drop=True)
# np.set_printoptions(threshold=1000)
# with pd.option_context('display.max_rows', None, 'display.max_columns', None):  # more options can be specified also
#     print(good_data)



print("Question 4:")
print("有，共5筆資料出現在不只一個feature中。")
print("這些資料屬於多種features，應該移除。")
print("因為k-means會被這些outliers影響，如果不將雜訊移除，可能會對後續分析出來的結果造成影響。")
print("\n", end='')
print("\n", end='')






from sklearn.decomposition import PCA
def display_pca_results(good_data, pca):
	'''
	Create a DataFrame of the PCA results
	Includes dimension feature weights and explained variance
	Visualizes the PCA results
	'''

	# Dimension indexing
	dimensions = dimensions = ['Dimension {}'.format(i) for i in range(1,len(pca.components_)+1)]

	# PCA components
	components = pd.DataFrame(np.round(pca.components_, 4), columns = list(good_data.keys()))
	components.index = dimensions

	# PCA explained variance
	ratios = pca.explained_variance_ratio_.reshape(len(pca.components_), 1)
	variance_ratios = pd.DataFrame(np.round(ratios, 4), columns = ['Explained Variance'])
	variance_ratios.index = dimensions

	# Create a bar plot visualization
	fig, ax = plt.subplots(figsize = (14,8))

	# Plot the feature weights as a function of the components
	components.plot(ax = ax, kind = 'bar');
	ax.set_ylabel("Feature Weights")
	ax.set_xticklabels(dimensions, rotation=0)


	# Display the explained variance ratios
	for i, ev in enumerate(pca.explained_variance_ratio_):
		ax.text(i-0.40, ax.get_ylim()[1] + 0.05, "Explained Variance\n          %.4f"%(ev))

	# Return a concatenated DataFrame
	return pd.concat([variance_ratios, components], axis = 1)

# TODO: Apply PCA by fitting the good data with the same number of dimensions as features
pca = PCA(n_components=6).fit(good_data)

# TODO: Transform log_samples using the PCA fit above
pca_samples = pca.transform(log_samples)

# Generate PCA results plot
pca_results = display_pca_results(good_data, pca)



print("Question 5:")
print("1. 0.4430 + 0.2638 = 0.7068 (70%)")
print("2. 93%")
print("3.")
print("Dimension1: Detergents_Paper, Grocery, Milk有較小變異數，這些東西購買需求較大，可能為Retailer")
print("Dimension2: Fresh, Frozen, Delicatessen有較小變異數，這些東西購買需求較大，可能為Restaurant")
print("Dimension3: Fresh, Detergents_Paper有較小變異數，這些東西購買需求較大，可能為Cafe")
print("Dimension4: Fresh, Delicatessen有較小變異數，這些東西購買需求較大，可能為Cafe")
print("\n", end='')
print("\n", end='')

# Display sample log-data after having a PCA transformation applied
display(pd.DataFrame(np.round(pca_samples, 4), columns = pca_results.index.values))





# TODO: Apply PCA by fitting the good data with only two dimensions
pca = PCA(n_components = 2, random_state=0)
pca.fit(good_data)

# TODO: Transform the good data using the PCA fit above
reduced_data = pca.transform(good_data)

# TODO: Transform log_samples using the PCA fit above
pca_samples = pca.transform(log_samples)

# Create a DataFrame for the reduced data
reduced_data = pd.DataFrame(reduced_data, columns = ['Dimension 1', 'Dimension 2'])

# Display sample log-data after applying PCA transformation in two dimensions
display(pd.DataFrame(np.round(pca_samples, 4), columns = ['Dimension 1', 'Dimension 2']))






def biplot(good_data, reduced_data, pca):
    '''
    Produce a biplot that shows a scatterplot of the reduced
    data and the projections of the original features.
    
    good_data: original data, before transformation.
               Needs to be a pandas dataframe with valid column names
    reduced_data: the reduced data (the first two dimensions are plotted)
    pca: pca object that contains the components_ attribute

    return: a matplotlib AxesSubplot object (for any additional customization)
    
    This procedure is inspired by the script:
    https://github.com/teddyroland/python-biplot
    '''

    fig, ax = plt.subplots(figsize = (14,8))
    # scatterplot of the reduced data    
    ax.scatter(x=reduced_data.loc[:, 'Dimension 1'], y=reduced_data.loc[:, 'Dimension 2'], 
        facecolors='b', edgecolors='b', s=70, alpha=0.5)
    
    feature_vectors = pca.components_.T

    # we use scaling factors to make the arrows easier to see
    arrow_size, text_pos = 7.0, 8.0,

    # projections of the original features
    for i, v in enumerate(feature_vectors):
        ax.arrow(0, 0, arrow_size*v[0], arrow_size*v[1], 
                  head_width=0.2, head_length=0.2, linewidth=2, color='red')
        ax.text(v[0]*text_pos, v[1]*text_pos, good_data.columns[i], color='black', 
                 ha='center', va='center', fontsize=18)

    ax.set_xlabel("Dimension 1", fontsize=14)
    ax.set_ylabel("Dimension 2", fontsize=14)
    ax.set_title("PC plane with original feature projections.", fontsize=16)
    plt.show()
    return ax

# Create a biplot
biplot(good_data, reduced_data, pca)
print("Dimension1: Detergents_Paper, Grocery, Milk, 三者都靠很近，表示三者有強烈關聯。")
print("Dimension2: Fresh, Frozen, Delicatessen。 Fresh, Frozen快要重和，兩者之間有強烈關連性。")
print("與前面的假設大致相同。")





print("Question 6:")
print("K-Means:假想分為k群，每一群都有中心點(k個)。每個點與k個中心點比較，將每一點分類為距離最近的中心點族群，並取得交界線。")
print("經過多次運算收斂直到交界線不再變動，得到最終分類。")
print("優點:理論簡單，好實現，且效率高，適合巨量資料。")
print("Gaussian Mixture Model:將各種抽樣出來的數據形成的多個高斯分布圖形，彼此取聯及所得到的最終圖形為Gaussian Mixture Model。")
print("優點:彈性高，單筆資料可以屬於多種群集。")
print("我會選擇K-Means，因為只有6個特徵，且比較好implement。")




from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
    
clusters_range = range(2,5)
for i in clusters_range:
# TODO: Apply your clustering algorithm of choice to the reduced data 
    clusterer = KMeans(n_clusters=i, random_state=0 )
    clusterer.fit(reduced_data)

# TODO: Predict the cluster for each data point
    preds = clusterer.predict(reduced_data)

# TODO: Find the cluster centers
    centers = clusterer.cluster_centers_

# TODO: Predict the cluster for each transformed sample data point
    sample_preds = clusterer.predict(pca_samples)

# TODO: Calculate the mean silhouette coefficient for the number of clusters chosen
    score = silhouette_score(reduced_data, preds)
    print("silhouette coefficient for `{}` clusters => {:.4f}".format(i, score))

print("Question 7:")
print("2 clusters: 0.4263")
print("3 clusters: 0.3974")
print("4 clusters: 0.3312")
print("2 clusters分數最高")