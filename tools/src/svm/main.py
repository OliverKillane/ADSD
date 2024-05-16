import src.svm.cli as cli
import src.svm.extract as extract
import src.svm.model as model


def main():
    try:
        args = cli.parse_args()

        test, train = extract.load_dataset(args.train_test_data)

        match args.source:
            case cli.Train(save=save, labels=labels):
                test_filter, train_filter = extract.filter_labels(test, train, labels)
                trained_model = model.train_model(train_filter)
                model_lab = model.SavedModel(trained_model, labels)
                if save is not None:
                    model.save_model(model_lab, save)
            case cli.Load(saved=saved):
                model_lab = model.load_model(saved)
                test_filter, train_filter = extract.filter_labels(
                    test, train, model_lab.labels
                )

        if args.test:
            model.test_model(test_filter, model_lab.model)

        if args.output is not None:
            cli.output_values(
                args.output, model.generate_model_quants(model_lab.model, test_filter)
            )
    except KeyboardInterrupt:
        print("Interrupted - quitting!")
