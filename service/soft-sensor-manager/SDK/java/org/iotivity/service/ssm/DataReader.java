package org.iotivity.service.ssm;

import java.util.List;

public class DataReader {

	private int pDataReaderInstance;

	public DataReader(int dataReaderInstance) {
		pDataReaderInstance = dataReaderInstance;
	}

	public List<String> GetAffectedModels() {
		return CoreController.getInstance().GetAffectedModels(
				pDataReaderInstance);
	}

	public int GetModelDataCount(String modelName) throws Exception {
		return CoreController.getInstance().GetModelDataCount(
				pDataReaderInstance, modelName);
	}

	public ModelData GetModelData(String modelName, int dataIndex)
			throws Exception {
		return CoreController.getInstance().GetModelData(pDataReaderInstance,
				modelName, dataIndex);
	}
}
