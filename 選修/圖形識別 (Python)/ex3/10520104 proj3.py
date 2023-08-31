import os
import sys
# Import libraries necessary for this project
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
pca = PCA(n_components=len(good_data.keys())).fit(good_data)

# TODO: Transform log_samples using the PCA fit above
pca_samples = pca.transform(log_samples)

# Generate PCA results plot
pca_results = rs.pca_results(good_data, pca)





